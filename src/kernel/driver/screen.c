#include "screen.h"
#include "ascii.h"

// The VGA framebuffer starts at 0xB8000.
int16_t *video_memory = (int16_t *)0xB8000;
// Stores the cursor position.

#define SCREEN_SIZE_X 80
#define SCREEN_SIZE_Y 25

uint8_t screen_cursor_x = 0;
uint8_t screen_cursor_y = 0;
uint8_t screen_settings;

void screen_resetSettings() {
    screen_setColours(BLACK, WHITE);
}

void screen_setBackColour(enum Colour colour) {
    uint8_t backColour = (uint8_t)colour;
    screen_settings = (backColour << 4) | (screen_settings & 0x0F);
}

void screen_setForeColour(enum Colour colour) {
    uint8_t foreColour = (uint8_t)colour;
    screen_settings = (screen_settings << 4) | (foreColour & 0x0F);
}

void screen_setColours(enum Colour colourBack, enum Colour colourFore) {
    uint8_t backColour = (uint8_t)colourBack;
    uint8_t foreColour = (uint8_t)colourFore;
    screen_settings = (backColour << 4) | (foreColour & 0x0F);
}

int16_t screen_getCharWithSettings(uint8_t c, uint8_t settings) {
    return c | (settings << 8);
}

int16_t screen_getCharWithCurrentSettings(uint8_t c) {
    return screen_getCharWithSettings(c, screen_settings);
}

// Updates the hardware cursor.
static void move_cursor()
{
    // The screen is SCREEN_SIZE_X characters wide...
    int16_t cursorLocation = screen_cursor_y * SCREEN_SIZE_X + screen_cursor_x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
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

// Writes a single character out to the screen.
void screen_put(char c)
{
    int16_t *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == ASCII_BACKSPACE) {
        if(screen_cursor_x) {
            screen_cursor_x--;
        } else if (screen_cursor_y) {
            screen_cursor_x=SCREEN_SIZE_X-1;
            screen_cursor_y--;
        }
        location = video_memory + (screen_cursor_y*SCREEN_SIZE_X + screen_cursor_x);
        *location = screen_getCharWithCurrentSettings(' ');
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == '\t')
    {
        screen_cursor_x = (screen_cursor_x+8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        screen_cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        screen_cursor_x = 0;
        screen_cursor_y++;
    }
    // Handle any other printable character.
    else if(c >= ' ')
    {
        location = video_memory + (screen_cursor_y*SCREEN_SIZE_X + screen_cursor_x);
        *location = screen_getCharWithCurrentSettings(c);
        screen_cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (screen_cursor_x >= SCREEN_SIZE_X)
    {
        screen_cursor_x = 0;
        screen_cursor_y ++;
    }

    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();

}

// Clears the screen, by copying lots of spaces to the framebuffer.
void screen_clear()
{
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

// Outputs a null-terminated ASCII string to the screen.
void screen_write(char *string)
{
    int i = 0;
    while (string[i])
    {
        screen_put(string[i++]);
    }
}
