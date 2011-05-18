#include "screen.h"

// The VGA framebuffer starts at 0xB8000.
int16_t *video_memory = (int16_t *)0xB8000;
// Stores the cursor position.

#define SCREEN_SIZE_X 80
#define SCREEN_SIZE_Y 25

#define BACKSPACE '\x08'
#define ESC '\x1B'

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

uint8_t backColour = BLACK;
uint8_t foreColour = WHITE;

void screen_setBackColour(enum Colour colour) {
    backColour = (uint8_t)colour;
}

void screen_setForeColour(enum Colour colour) {
    foreColour = (uint8_t)colour;
}

void screen_setColours(enum Colour colourBack, enum Colour colourFore) {
    screen_setBackColour(colourBack);
    screen_setForeColour(colourFore);
}

// Updates the hardware cursor.
static void move_cursor()
{
    // The screen is SCREEN_SIZE_X characters wide...
    int16_t cursorLocation = cursor_y * SCREEN_SIZE_X + cursor_x;
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
    if(cursor_y >= SCREEN_SIZE_Y)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*SCREEN_SIZE_X; i < (SCREEN_SIZE_Y-1)*SCREEN_SIZE_X; i++)
        {
            video_memory[i] = video_memory[i+SCREEN_SIZE_X];
        }

        // The last line should now be blank. Do this by writing
        // SCREEN_SIZE_X spaces to it.
        for (i = (SCREEN_SIZE_Y-1)*SCREEN_SIZE_X; i < SCREEN_SIZE_Y*SCREEN_SIZE_X; i++)
        {
            video_memory[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = (SCREEN_SIZE_Y-1);
    }
}

// Writes a single character out to the screen.
void screen_put(char c)
{

    // The attribute byte is made up of two nibbles - the lower being the
    // foreground colour, and the upper the background colour.
    uint8_t  attributeByte = (backColour << 4) | (foreColour & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    int16_t attribute = attributeByte << 8;
    int16_t *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == BACKSPACE) {
        if(cursor_x) {
            cursor_x--;
        } else if (cursor_y) {
            cursor_x=SCREEN_SIZE_X-1;
            cursor_y--;
        }
        location = video_memory + (cursor_y*SCREEN_SIZE_X + cursor_x);
        *location = ' ' | attribute;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == '\t')
    {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    // Handle any other printable character.
    else if(c >= ' ')
    {
        location = video_memory + (cursor_y*SCREEN_SIZE_X + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= SCREEN_SIZE_X)
    {
        cursor_x = 0;
        cursor_y ++;
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
    cursor_x = 0;
    cursor_y = 0;
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

void screen_write_hex(int32_t n)
{
    // TODO: implement this yourself!
}

void screen_write_dec(int32_t n)
{
    // TODO: implement this yourself!
}
