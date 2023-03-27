#include "keyboard.h"

static struct KeyboardDriverState keyboard_state;

const char keyboard_scancode_1_to_ascii_map[256] = {
      0, 0x1B, '1', '2', '3', '4', '5', '6',  '7', '8', '9',  '0',  '-', '=', '\b', '\t',
    'q',  'w', 'e', 'r', 't', 'y', 'u', 'i',  'o', 'p', '[',  ']', '\n',   0,  'a',  's',
    'd',  'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0, '\\',  'z', 'x',  'c',  'v',
    'b',  'n', 'm', ',', '.', '/',   0, '*',    0, ' ',   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0, '-',    0,    0,   0,  '+',    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,

      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
      0,    0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,    0,   0,    0,    0,
};

void keyboard_state_activate(void) {
    keyboard_state.keyboard_input_on = TRUE;
}

// Deactivate keyboard ISR / stop listening keyboard interrupt
void keyboard_state_deactivate(void) {
    keyboard_state.keyboard_input_on = FALSE;
}

// Get keyboard buffer values - @param buf Pointer to char buffer, recommended size at least KEYBOARD_BUFFER_SIZE
void get_keyboard_buffer(char *buf) {
    memcpy(buf, keyboard_state.keyboard_buffer, KEYBOARD_BUFFER_SIZE);
}

// Check whether keyboard ISR is active or not - @return Equal with keyboard_input_on value
bool is_keyboard_blocking(void) {
    return keyboard_state.keyboard_input_on;
}

void keyboard_isr(void) {
    if (!keyboard_state.keyboard_input_on) {
        keyboard_state.buffer_index = 0;
    } 
    else {
        uint8_t  scancode    = in(KEYBOARD_DATA_PORT);
        char     mapped_char = keyboard_scancode_1_to_ascii_map[scancode];
        uint8_t  cursor_x, cursor_y;
        bool     process_scancode = TRUE;
        if (mapped_char != 0) {
            switch (mapped_char) {
                case '\n': // Enter key
                    keyboard_state.keyboard_buffer[keyboard_state.buffer_index++] = mapped_char;
                    process_scancode = FALSE;
                    break;
                case '\b': // Backspace key
                    if (keyboard_state.buffer_index > 0) {
                        keyboard_state.buffer_index--;
                        framebuffer_get_cursor(&cursor_x, &cursor_y);
                        framebuffer_set_cursor(cursor_x, cursor_y-1);
                        framebuffer_write(cursor_x, cursor_y-1, ' ', 0x0F, 0x00);
                        framebuffer_set_cursor(cursor_x, cursor_y-1);
                    }
                    process_scancode = FALSE;
                    break;
                default:
                    keyboard_state.keyboard_buffer[keyboard_state.buffer_index++] = mapped_char;
                    framebuffer_get_cursor(&cursor_x, &cursor_y);
                    framebuffer_write(cursor_x, cursor_y, mapped_char, 0x0F, 0x00);
                    framebuffer_set_cursor(cursor_x, cursor_y+1);
                    break;
            }
        }
        
        // Process extended scancodes (arrow keys)
        if (process_scancode && scancode == EXTENDED_SCANCODE_BYTE) {
            keyboard_state.read_extended_mode = TRUE;
        } 
        else if (process_scancode && keyboard_state.read_extended_mode) {
            switch (scancode) {
                case EXT_SCANCODE_UP:
                    //belom
                    break;
                case EXT_SCANCODE_DOWN:
                    //belom
                    break;
                case EXT_SCANCODE_LEFT:
                    //belom
                    break;
                case EXT_SCANCODE_RIGHT:
                    //belom
                    break;
                default:

                    break;
            }
            keyboard_state.read_extended_mode = FALSE;
        }
    }
    pic_ack(IRQ_KEYBOARD);
}
