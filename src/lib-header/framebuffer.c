#include "framebuffer.h"

void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint16_t* cell = (uint16_t*)(MEMORY_FRAMEBUFFER + 2 * (row * 80 + col));
    *cell = (uint16_t)c | ((uint16_t)fg << 8) | ((uint16_t)bg << 12);
}

void framebuffer_set_cursor(uint8_t r, uint8_t c) {
    uint16_t pos = r * 80 + c;
    out(CURSOR_PORT_CMD, 0x0F);
    out(CURSOR_PORT_DATA, (uint8_t)(pos & 0xFF));
    out(CURSOR_PORT_CMD, 0x0E);
    out(CURSOR_PORT_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

void framebuffer_clear(void) {
    memset(MEMORY_FRAMEBUFFER, 0, 80 * 25 * 2);
    for (uint8_t row = 0; row < 25; row++) {
        for (uint8_t col = 0; col < 80; col++) {
            framebuffer_write(row, col, ' ', 0x07, 0x00);
        }
    }
}

void framebuffer_get_cursor(uint8_t* row, uint8_t* col) {
    out(CURSOR_PORT_CMD, 0x0F);
    uint16_t pos_low = in(CURSOR_PORT_DATA);
    out(CURSOR_PORT_CMD, 0x0E);
    uint16_t pos_high = in(CURSOR_PORT_DATA);
    uint16_t pos = (pos_high << 8) | pos_low;
    *row = pos / 80;
    *col = pos % 80;
}