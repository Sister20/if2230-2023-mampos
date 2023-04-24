#include "keyboard.h"

static struct KeyboardDriverState keyboard_state;

const char keyboard_scancode_1_to_ascii_map[256] = {
    0,
    0x1B,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    '\b',
    '\t',
    'q',
    'w',
    'e',
    'r',
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n',
    'X',
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',
    '\'',
    '`',
    'X',
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',
    'm',
    ',',
    '.',
    '/',
    0,
    '*',
    0,
    ' ',
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '-',
    0,
    0,
    0,
    '+',
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // default
    0,
    0,
    0,
    0,
    0,
};

void keyboard_state_activate(void)
{
    keyboard_state.keyboard_input_on = TRUE;
}

// Deactivate keyboard ISR / stop listening keyboard interrupt
void keyboard_state_deactivate(void)
{
    keyboard_state.keyboard_input_on = FALSE;
}

// Get keyboard buffer values - @param buf Pointer to char buffer, recommended size at least KEYBOARD_BUFFER_SIZE
void get_keyboard_buffer(char *buf)
{
    memcpy(buf, keyboard_state.keyboard_buffer, KEYBOARD_BUFFER_SIZE);
}

// Check whether keyboard ISR is active or not - @return Equal with keyboard_input_on value
bool is_keyboard_blocking(void)
{
    return keyboard_state.keyboard_input_on;
}

void keyboard_isr(void)
{

    uint8_t cursor_x, cursor_y;
    uint8_t scancode = in(KEYBOARD_DATA_PORT);
    char mapped_char = keyboard_scancode_1_to_ascii_map[scancode];
    static char last_mapped_char = 0;
    static bool make_code = FALSE;

    if (!is_keyboard_blocking())
    {
        keyboard_state.buffer_index = 0;
    }
    else
    {
        // Register mapped character when pressed
        if (mapped_char != 0)
        {
            make_code = TRUE;
            last_mapped_char = mapped_char;
        }
        // Put mapped character in buffer after release
        else if (make_code)
        {
            framebuffer_get_cursor(&cursor_x, &cursor_y);

            // Backspace
            if (last_mapped_char == '\b')
            {
                if (keyboard_state.buffer_index > 0)
                {
                    if (cursor_y != 0)
                    {
                        keyboard_state.keyboard_buffer[keyboard_state.buffer_index] = 0;
                        keyboard_state.buffer_index--;
                        framebuffer_set_cursor(cursor_x, cursor_y - 1);
                        framebuffer_write(cursor_x, cursor_y - 1, ' ', 0x0F, 0x00);
                    }
                    if (cursor_y == 0) 
                    {
                        if (cursor_x > 0)
                        {
                            keyboard_state.keyboard_buffer[keyboard_state.buffer_index] = 0;
                            keyboard_state.buffer_index--;
                            framebuffer_set_cursor(cursor_x-1, 79);
                            framebuffer_write(cursor_x-1, 79, ' ', 0x0F, 0x00);

                        } 
                    }
                }
            }

            // Enter
            else if (last_mapped_char == '\n')
            {
                keyboard_state.keyboard_buffer[keyboard_state.buffer_index++] = last_mapped_char;
                framebuffer_set_cursor(cursor_x + 1, 0);
                keyboard_state_deactivate();
            }

            // Character
            else if (last_mapped_char != 0)
            {
                keyboard_state.keyboard_buffer[keyboard_state.buffer_index++] = last_mapped_char;
                framebuffer_write(cursor_x, cursor_y, last_mapped_char, 0x0F, 0x00);
                framebuffer_set_cursor(cursor_x, cursor_y + 1);
                last_mapped_char = 0;
            }
            make_code = FALSE;
        }
        // Arrow
        else if (scancode == EXT_SCANCODE_UP)
        {
            framebuffer_get_cursor(&cursor_x, &cursor_y);
            if (cursor_x > 0) {
                framebuffer_set_cursor(cursor_x-1, cursor_y);
            }
        }
        else if (scancode == EXT_SCANCODE_DOWN)
        {
            framebuffer_get_cursor(&cursor_x, &cursor_y);
            if (cursor_x < 24) {
                framebuffer_set_cursor(cursor_x+1, cursor_y);
            }
        }
        else if (scancode == EXT_SCANCODE_LEFT)
        {
            framebuffer_get_cursor(&cursor_x, &cursor_y);
            if (cursor_y > 0) {
                framebuffer_set_cursor(cursor_x, cursor_y-1);
            } else {
                if(cursor_x > 0) {
                    framebuffer_set_cursor(cursor_x-1, 79);
                }
            }
        }
        else if (scancode == EXT_SCANCODE_RIGHT)
        {
            framebuffer_get_cursor(&cursor_x, &cursor_y);
            if (cursor_y < 79) {
                framebuffer_set_cursor(cursor_x, cursor_y+1);
            }
        }
    }

    pic_ack(IRQ_KEYBOARD);
}
