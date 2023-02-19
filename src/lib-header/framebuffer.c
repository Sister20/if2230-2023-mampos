#include "lib-header/framebuffer.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/portio.h"

void framebuffer_set_cursor(uint8_t r, uint8_t c) {
    
}

void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint16_t* cell = (uint16_t*)(MEMORY_FRAMEBUFFER + 2 * (row * 80 + col));
    *cell = (uint16_t)c | ((uint16_t)fg << 8) | ((uint16_t)bg << 12);
}

void framebuffer_clear(void) {
    // TODO : Implement
}
