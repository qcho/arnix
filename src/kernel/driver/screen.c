#include "screen.h"
#include "../system/isr.h"
#include "../system/in_out.h"
#include "ascii.h"

// The VGA framebuffer starts at 0xB8000.
int16_t *video_memory = (int16_t *)0xB8000;
// Stores the cursor position.

#define BUFFER_SIZE 1000

char array_out[BUFFER_SIZE];

buffer_t stdout;

#define ESC '\x1B'
#define BELL '\x07'

#define DEFAULT_SETTINGS 0x07

#define SCREEN_SIZE_X 80
#define SCREEN_SIZE_Y 25
uint8_t screen_state = 0; // 0=normal, 1=scaped, 2=parameters.

#define SCREEN_MAX_PARAM_COUNT 16
uint8_t screen_param_count = 0;
int screen_param[SCREEN_MAX_PARAM_COUNT];

uint8_t screen_cursor_x = 0;
uint8_t screen_cursor_y = 0;
uint8_t screen_settings = DEFAULT_SETTINGS;

// Updates the hardware cursor.
PRIVATE void move_cursor()
{
    // The screen is SCREEN_SIZE_X characters wide...
    int16_t cursorLocation = screen_cursor_y * SCREEN_SIZE_X + screen_cursor_x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
PRIVATE void scroll()
{

    // Get a space character with the default colour attributes.
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    int16_t blank = 0x20 /* space */ | (attributeByte << 8);

    // Row SCREEN_SIZE_Y is the end, this means we need to scroll up
    if(screen_cursor_y >= SCREEN_SIZE_Y)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*SCREEN_SIZE_X; i < (SCREEN_SIZE_Y-1)*SCREEN_SIZE_X; i++)
        {
            video_memory[i] = video_memory[i+SCREEN_SIZE_X];
        }
        int lastLine = SCREEN_SIZE_Y-1;
        // The last line should now be blank. Do this by writing
        // SCREEN_SIZE_X spaces to it.
        for (i = (lastLine)*SCREEN_SIZE_X; i < SCREEN_SIZE_Y*SCREEN_SIZE_X; i++)
        {
            video_memory[i] = blank;
        }
        screen_cursor_y = (lastLine);
    }
}

PRIVATE void print(char c) {
    int16_t *location;
    location = video_memory + (screen_cursor_y*SCREEN_SIZE_X + screen_cursor_x);
    
    if (c != '\b') {
        *location = (c | (screen_settings << 8));
        if (++screen_cursor_x >= SCREEN_SIZE_X) {
            screen_cursor_x = 0;
            screen_cursor_y ++;
        }
    } else {
        *location = (' ' | (screen_settings << 8));
    }
}

PRIVATE void do_bell() {
   // TODO
}

PRIVATE void do_backspace() {
    if(screen_cursor_x) {
        screen_cursor_x--;
    } else if (screen_cursor_y) {
        screen_cursor_x=SCREEN_SIZE_X-1;
        screen_cursor_y--;
    }
    print('\b');
}

PRIVATE void do_lineFeed() {
    screen_cursor_x = 0;
    screen_cursor_y++;
}

PRIVATE void do_tab() {
    screen_cursor_x = (screen_cursor_x+8) & ~(8-1);
}

PRIVATE void do_return() {
    screen_cursor_x = 0;
}

// Clears the screen, by copying lots of spaces to the framebuffer.
PRIVATE void screen_clear() {
    // Make an attribute byte for the default colours
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    int16_t blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < SCREEN_SIZE_X*SCREEN_SIZE_Y; i++)
    {
        video_memory[i] = blank;
    }

    // Move the hardware cursor back to the start.
    screen_cursor_x = 0;
    screen_cursor_y = 0;
    move_cursor();
}

PRIVATE void do_scape_J() {
    if (screen_param[0] == 2) {
        screen_clear();
    }
}

/* Map from ANSI colors to the attributes used by the PC */
PRIVATE int ansi_colors[8] = {0, 4, 2, 6, 1, 5, 3, 7};
    
PRIVATE void do_scape_m() {
    int i;
    for (i=0;i<screen_param_count;i++){
        int dec = screen_param[i]/10;
        int u = screen_param[i]%10;
        if (dec == 0) {
            switch(u){
                case 0:
                    screen_settings = DEFAULT_SETTINGS;
                    break;
                case 1:
                    screen_settings |= 0x08;
                    break;
                case 4:
                    screen_settings &= 0xBB;
                    break;
                case 5:
                    screen_settings |= 0x80;
            }
        } else if (dec == 3) { /* background */
            screen_settings = (screen_settings << 4) | (ansi_colors[u] & 0x0F);
        } else if (dec == 4) { /* foreground */
            screen_settings = (ansi_colors[u] << 4) | (screen_settings & 0x0F);
        }
    }
}

PRIVATE void do_scape(char c) {
    switch(screen_state) {
        case 1:
            if (c == '[') {
                screen_state = 2;
                screen_param_count = 1;
                int i=0;
                for (;i<=SCREEN_MAX_PARAM_COUNT; i++) {
                    screen_param[i] = 0;
                }
            } else {
                screen_state = 0;
            }
            break;
        case 2:
            if (c >= '0' && c <= '9') {
                screen_param[screen_param_count-1] = 10*screen_param[screen_param_count-1] + (c-'0');
            } else if (c == ';') {
                screen_param_count++;
            } else {
                switch (c) {
                    case 'm':
                        do_scape_m();
                        break;
                    case 'J':
                        do_scape_J();
                        break;
                }
                screen_state = 0;
            }  
            break;
    }
}

// Writes a single character out to the screen.
PUBLIC void screen_put(char c) {
    if (screen_state > 0) {
        do_scape(c);
        return;
    } else {
        switch (c) {
            case ESC:
                screen_state = 1;
                return;
            case '\0':
                return;
            case BELL:
                do_bell();
                return;
            case '\b':
                do_backspace();
                break;
            case '\n':
                do_lineFeed();
                break;
            case '\t':
                do_tab();
                break;
            case '\r':
                do_return();
                break;
            default:
                print(c);
                break;
        }
        scroll();
        move_cursor();
    }
}

PUBLIC void screen_write(char *string) {
    int i = 0;
    while (string[i]) {
        screen_put(string[i++]);
    }
}

PRIVATE void IRQ0_handler(registers_t reg){
	int i;
	for(i=0;stdout.start!=stdout.end;i++){
		screen_put(stdout.array[stdout.start]);
		stdout.start=(stdout.start+1)%stdout.size;
	}
}

PUBLIC void init_screen(){
	register_interrupt_handler(IRQ0,IRQ0_handler);
	stdout.start=stdout.end=0;
	stdout.array=array_out;
	stdout.size=BUFFER_SIZE;
	add_in_out(1,&stdout);
        screen_write("\x1B[2J");
        screen_write("\x1B[34;47m");
}
