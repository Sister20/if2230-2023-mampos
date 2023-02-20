#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.h"
#include "lib-header/framebuffer.h"
#include "lib-header/kernel_loader.h"

void kernel_setup(void) {
    framebuffer_clear();
    framebuffer_write(3, 8,  ' ', 0, 0x1);
    framebuffer_write(3, 9,  ' ', 0, 0x2);
    framebuffer_write(3, 10, ' ', 0, 0x3);
    framebuffer_write(3, 11, ' ', 0, 0x4);
    framebuffer_write(4, 8,  ' ', 0, 0x5);
    framebuffer_write(4, 9,  ' ', 0, 0x6);
    framebuffer_write(4, 10, ' ', 0, 0x7);
    framebuffer_write(4, 11, ' ', 0, 0x8);
    framebuffer_write(4, 12,  ' ', 0, 0x9);
    framebuffer_write(4, 13,  ' ', 0, 0xA);
    framebuffer_write(4, 14, ' ', 0, 0xB);
    framebuffer_write(5, 8, ' ', 0, 0xC);
    framebuffer_write(5, 9, ' ', 0, 0xD);
    framebuffer_write(5, 10, ' ', 0, 0xE);
    framebuffer_write(5, 11, ' ', 0, 0xF);
    framebuffer_write(5, 12, ' ', 0, 0x0);


    framebuffer_set_cursor(7, 13);

    while (TRUE);
}
