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


    framebuffer_write(8, 11, ' ', 0, 0x8);//row 1
    framebuffer_write(8, 12, ' ', 0, 0x8);
    framebuffer_write(8, 13, ' ', 0, 0x8);
    framebuffer_write(8, 14, ' ', 0, 0x8);
    framebuffer_write(8, 15, ' ', 0, 0x8);
    framebuffer_write(8, 16, ' ', 0, 0x8);
    framebuffer_write(8, 17, ' ', 0, 0x8);
    framebuffer_write(8, 18, ' ', 0, 0x8);
    framebuffer_write(8, 19, ' ', 0, 0x8);
    framebuffer_write(8, 20, ' ', 0, 0x8);
    framebuffer_write(8, 21, ' ', 0, 0x8);
    framebuffer_write(8, 22, ' ', 0, 0x8);
    framebuffer_write(8, 23, ' ', 0, 0x8);
    framebuffer_write(8, 24, ' ', 0, 0x8);

    framebuffer_write(9, 11, ' ', 0, 0x8);//row 2
    framebuffer_write(9, 12, ' ', 0, 0x8);
    framebuffer_write(9, 13, ' ', 0, 0xD);
    framebuffer_write(9, 14, ' ', 0, 0xD);
    framebuffer_write(9, 15, ' ', 0, 0xD);
    framebuffer_write(9, 16, ' ', 0, 0xD);
    framebuffer_write(9, 17, ' ', 0, 0x5);
    framebuffer_write(9, 18, ' ', 0, 0x5);
    framebuffer_write(9, 19, ' ', 0, 0x5);
    framebuffer_write(9, 20, ' ', 0, 0x5);
    framebuffer_write(9, 21, ' ', 0, 0x5);
    framebuffer_write(9, 22, ' ', 0, 0x5);
    framebuffer_write(9, 23, ' ', 0, 0x8);
    framebuffer_write(9, 24, ' ', 0, 0x8);

    framebuffer_write(10, 11, ' ', 0, 0x8);//row 3
    framebuffer_write(10, 12, ' ', 0, 0x8);
    framebuffer_write(10, 13, ' ', 0, 0xD);
    framebuffer_write(10, 14, ' ', 0, 0xD);
    framebuffer_write(10, 15, ' ', 0, 0xD);
    framebuffer_write(10, 16, ' ', 0, 0xD);
    framebuffer_write(10, 17, ' ', 0, 0x5);
    framebuffer_write(10, 18, ' ', 0, 0x5);
    framebuffer_write(10, 19, ' ', 0, 0x5);
    framebuffer_write(10, 20, ' ', 0, 0x5);
    framebuffer_write(10, 21, ' ', 0, 0x5);
    framebuffer_write(10, 22, ' ', 0, 0x5);
    framebuffer_write(10, 23, ' ', 0, 0x8);
    framebuffer_write(10, 24, ' ', 0, 0x8);

    framebuffer_write(11, 11, ' ', 0, 0x8);//row 4
    framebuffer_write(11, 12, ' ', 0, 0x8);
    framebuffer_write(11, 13, ' ', 0, 0x8);
    framebuffer_write(11, 14, ' ', 0, 0x8);
    framebuffer_write(11, 15, ' ', 0, 0x8);
    framebuffer_write(11, 16, ' ', 0, 0x8);
    framebuffer_write(11, 17, ' ', 0, 0x8);
    framebuffer_write(11, 18, ' ', 0, 0x8);
    framebuffer_write(11, 19, ' ', 0, 0x8);
    framebuffer_write(11, 20, ' ', 0, 0x8);
    framebuffer_write(11, 21, ' ', 0, 0x8);
    framebuffer_write(11, 22, ' ', 0, 0x8);
    framebuffer_write(11, 23, ' ', 0, 0x8);
    framebuffer_write(11, 24, ' ', 0, 0x8);
    framebuffer_write(11, 25, ' ', 0, 0x8);
    framebuffer_write(11, 26, ' ', 0, 0x8);
    framebuffer_write(11, 27, ' ', 0, 0x8);
    framebuffer_write(11, 28, ' ', 0, 0x8);
    framebuffer_write(11, 29, ' ', 0, 0x8);
    framebuffer_write(11, 30, ' ', 0, 0x8);
    
    framebuffer_write(12, 11, ' ', 0, 0x8);//row 5
    framebuffer_write(12, 12, ' ', 0, 0x8);
    framebuffer_write(12, 13, ' ', 0, 0xD);
    framebuffer_write(12, 14, ' ', 0, 0xD);
    framebuffer_write(12, 15, ' ', 0, 0xD);
    framebuffer_write(12, 16, ' ', 0, 0xD);
    framebuffer_write(12, 17, ' ', 0, 0x5);
    framebuffer_write(12, 18, ' ', 0, 0x5);
    framebuffer_write(12, 19, ' ', 0, 0x5);
    framebuffer_write(12, 20, ' ', 0, 0x5);
    framebuffer_write(12, 21, ' ', 0, 0x5);
    framebuffer_write(12, 22, ' ', 0, 0x5);
    framebuffer_write(12, 23, ' ', 0, 0x5);
    framebuffer_write(12, 24, ' ', 0, 0x5);
    framebuffer_write(12, 25, ' ', 0, 0x5);
    framebuffer_write(12, 26, ' ', 0, 0x5);
    framebuffer_write(12, 27, ' ', 0, 0x5);
    framebuffer_write(12, 28, ' ', 0, 0x5);
    framebuffer_write(12, 29, ' ', 0, 0x8);
    framebuffer_write(12, 30, ' ', 0, 0x8);

    framebuffer_write(13, 3, ' ', 0, 0x8);//row 6
    framebuffer_write(13, 4, ' ', 0, 0x8);
    framebuffer_write(13, 5, ' ', 0, 0x8);
    framebuffer_write(13, 6, ' ', 0, 0x8);
    framebuffer_write(13, 9, ' ', 0, 0x8);
    framebuffer_write(13, 10, ' ', 0, 0x8);
    framebuffer_write(13, 11, ' ', 0, 0x5);
    framebuffer_write(13, 12, ' ', 0, 0x5);
    framebuffer_write(13, 13, ' ', 0, 0x8);
    framebuffer_write(13, 14, ' ', 0, 0x8);
    framebuffer_write(13, 15, ' ', 0, 0xD);
    framebuffer_write(13, 16, ' ', 0, 0xD);
    framebuffer_write(13, 17, ' ', 0, 0xD);
    framebuffer_write(13, 18, ' ', 0, 0xD);
    framebuffer_write(13, 19, ' ', 0, 0x5);
    framebuffer_write(13, 20, ' ', 0, 0x5);
    framebuffer_write(13, 21, ' ', 0, 0x5);
    framebuffer_write(13, 22, ' ', 0, 0x5);
    framebuffer_write(13, 23, ' ', 0, 0x5);
    framebuffer_write(13, 24, ' ', 0, 0x5);
    framebuffer_write(13, 25, ' ', 0, 0x5);
    framebuffer_write(13, 26, ' ', 0, 0x5);
    framebuffer_write(13, 27, ' ', 0, 0x5);
    framebuffer_write(13, 28, ' ', 0, 0x5);
    framebuffer_write(13, 29, ' ', 0, 0x8);
    framebuffer_write(13, 30, ' ', 0, 0x8);

    framebuffer_write(14, 1, ' ', 0, 0x8);//row 7
    framebuffer_write(14, 2, ' ', 0, 0x8);
    framebuffer_write(14, 3, ' ', 0, 0x8);
    framebuffer_write(14, 4, ' ', 0, 0x8);
    framebuffer_write(14, 5, ' ', 0, 0xF);
    framebuffer_write(14, 6, ' ', 0, 0xF);
    framebuffer_write(14, 7, ' ', 0, 0x8);
    framebuffer_write(14, 8, ' ', 0, 0x8);
    framebuffer_write(14, 9, ' ', 0, 0x8);
    framebuffer_write(14, 10, ' ', 0, 0x8);
    framebuffer_write(14, 11, ' ', 0, 0x5);
    framebuffer_write(14, 12, ' ', 0, 0x5);
    framebuffer_write(14, 13, ' ', 0, 0x8);
    framebuffer_write(14, 14, ' ', 0, 0x8);
    framebuffer_write(14, 15, ' ', 0, 0xD);
    framebuffer_write(14, 16, ' ', 0, 0xD);
    framebuffer_write(14, 17, ' ', 0, 0xD);
    framebuffer_write(14, 18, ' ', 0, 0xD);
    framebuffer_write(14, 19, ' ', 0, 0xD);
    framebuffer_write(14, 20, ' ', 0, 0xD);
    framebuffer_write(14, 21, ' ', 0, 0x5);
    framebuffer_write(14, 22, ' ', 0, 0x5);
    framebuffer_write(14, 23, ' ', 0, 0x5);
    framebuffer_write(14, 24, ' ', 0, 0x5);
    framebuffer_write(14, 25, ' ', 0, 0x8);
    framebuffer_write(14, 26, ' ', 0, 0x8);
    framebuffer_write(14, 27, ' ', 0, 0x8);
    framebuffer_write(14, 28, ' ', 0, 0x8);
    framebuffer_write(14, 29, ' ', 0, 0x8);
    framebuffer_write(14, 30, ' ', 0, 0x8);

    framebuffer_write(15, 1, ' ', 0, 0x8);//row 8
    framebuffer_write(15, 2, ' ', 0, 0x8);
    framebuffer_write(15, 3, ' ', 0, 0xF);
    framebuffer_write(15, 4, ' ', 0, 0xF);
    framebuffer_write(15, 5, ' ', 0, 0xF);
    framebuffer_write(15, 6, ' ', 0, 0xF);
    framebuffer_write(15, 7, ' ', 0, 0xF);
    framebuffer_write(15, 8, ' ', 0, 0xF);
    framebuffer_write(15, 9, ' ', 0, 0x8);
    framebuffer_write(15, 10, ' ', 0, 0x8);
    framebuffer_write(15, 11, ' ', 0, 0x8);
    framebuffer_write(15, 12, ' ', 0, 0x8);
    framebuffer_write(15, 13, ' ', 0, 0x5);
    framebuffer_write(15, 14, ' ', 0, 0x5);
    framebuffer_write(15, 15, ' ', 0, 0x8);
    framebuffer_write(15, 16, ' ', 0, 0x8);
    framebuffer_write(15, 17, ' ', 0, 0xD);
    framebuffer_write(15, 18, ' ', 0, 0xD);
    framebuffer_write(15, 19, ' ', 0, 0xD);
    framebuffer_write(15, 20, ' ', 0, 0xD);
    framebuffer_write(15, 21, ' ', 0, 0xD);
    framebuffer_write(15, 22, ' ', 0, 0xD);
    framebuffer_write(15, 23, ' ', 0, 0x5);
    framebuffer_write(15, 24, ' ', 0, 0x5);
    framebuffer_write(15, 25, ' ', 0, 0x8);
    framebuffer_write(15, 26, ' ', 0, 0x8);


    framebuffer_write(16, 3, ' ', 0, 0x8);//row 9
    framebuffer_write(16, 4, ' ', 0, 0x8);
    framebuffer_write(16, 5, ' ', 0, 0xF);
    framebuffer_write(16, 6, ' ', 0, 0xF);
    framebuffer_write(16, 7, ' ', 0, 0xF);
    framebuffer_write(16, 8, ' ', 0, 0xF);
    framebuffer_write(16, 9, ' ', 0, 0xF);
    framebuffer_write(16, 10, ' ', 0, 0xF);
    framebuffer_write(16, 11, ' ', 0, 0xF);
    framebuffer_write(16, 12, ' ', 0, 0xF);
    framebuffer_write(16, 13, ' ', 0, 0xD);
    framebuffer_write(16, 14, ' ', 0, 0xD);
    framebuffer_write(16, 15, ' ', 0, 0x8);
    framebuffer_write(16, 16, ' ', 0, 0x8);
    framebuffer_write(16, 17, ' ', 0, 0xD);
    framebuffer_write(16, 18, ' ', 0, 0xD);
    framebuffer_write(16, 19, ' ', 0, 0xD);
    framebuffer_write(16, 20, ' ', 0, 0xD);
    framebuffer_write(16, 21, ' ', 0, 0xD);
    framebuffer_write(16, 22, ' ', 0, 0xD);
    framebuffer_write(16, 23, ' ', 0, 0x5);
    framebuffer_write(16, 24, ' ', 0, 0x5);
    framebuffer_write(16, 25, ' ', 0, 0x8);
    framebuffer_write(16, 26, ' ', 0, 0x8);



    framebuffer_set_cursor(7, 13);

    while (TRUE);
}
