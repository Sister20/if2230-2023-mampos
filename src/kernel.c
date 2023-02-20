#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.h"
#include "lib-header/framebuffer.h"
#include "lib-header/kernel_loader.h"

void kernel_setup(void) {
    framebuffer_clear();
    framebuffer_write(3, 8,  'H', 0, 0xF);
    framebuffer_write(3, 9,  'a', 0, 0xF);
    framebuffer_write(3, 10, 'i', 0, 0xF);
    framebuffer_write(3, 11, '!', 0, 0xF);
    framebuffer_write(4, 8,  'S', 0, 0xF);
    framebuffer_write(4, 9,  'e', 0, 0xF);
    framebuffer_write(4, 10, 'l', 0, 0xF);
    framebuffer_write(4, 11, 'a', 0, 0xF);
    framebuffer_write(4, 12,  'm', 0, 0xF);
    framebuffer_write(4, 13,  'a', 0, 0xF);
    framebuffer_write(4, 14, 't', 0, 0xF);
    framebuffer_write(5, 8, 'd', 0, 0xF);
    framebuffer_write(5, 9, 'a', 0, 0xF);
    framebuffer_write(5, 10, 't', 0, 0xF);
    framebuffer_write(5, 11, 'a', 0, 0xF);
    framebuffer_write(5, 12, 'n', 0, 0xF);
    framebuffer_write(5, 13, 'g', 0, 0xF);
    framebuffer_write(6, 8, 'd', 0, 0xF);
    framebuffer_write(6, 9, 'i', 0, 0xF);
    framebuffer_write(7, 8, 'm', 0, 0xF);
    framebuffer_write(7, 9, 'a', 0, 0xF);
    framebuffer_write(7, 10, 'm', 0, 0xF);
    framebuffer_write(7, 11, 'p', 0, 0xF);
    framebuffer_write(7, 12, 'O', 0, 3);
    framebuffer_write(7, 13, 'S', 0, 3);    

    framebuffer_set_cursor(7, 13);

    while (TRUE);
}
