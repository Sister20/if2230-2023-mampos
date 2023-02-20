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

    framebuffer_write(17, 1, ' ', 0, 0x8);//row 10
    framebuffer_write(17, 2, ' ', 0, 0x8);
    framebuffer_write(17, 3, ' ', 0, 0xF);
    framebuffer_write(17, 4, ' ', 0, 0xF);
    framebuffer_write(17, 5, ' ', 0, 0xF);
    framebuffer_write(17, 6, ' ', 0, 0xF);
    framebuffer_write(17, 7, ' ', 0, 0xF);
    framebuffer_write(17, 8, ' ', 0, 0xF);
    framebuffer_write(17, 9, ' ', 0, 0x8);
    framebuffer_write(17, 10, ' ', 0, 0x8);
    framebuffer_write(17, 11, ' ', 0, 0x8);
    framebuffer_write(17, 12, ' ', 0, 0x8);
    framebuffer_write(17, 13, ' ', 0, 0x5);
    framebuffer_write(17, 14, ' ', 0, 0x5);
    framebuffer_write(17, 15, ' ', 0, 0x8);
    framebuffer_write(17, 16, ' ', 0, 0x8);
    framebuffer_write(17, 17, ' ', 0, 0xD);
    framebuffer_write(17, 18, ' ', 0, 0xD);
    framebuffer_write(17, 19, ' ', 0, 0xD);
    framebuffer_write(17, 20, ' ', 0, 0xD);
    framebuffer_write(17, 21, ' ', 0, 0xD);
    framebuffer_write(17, 22, ' ', 0, 0xD);
    framebuffer_write(17, 23, ' ', 0, 0x5);
    framebuffer_write(17, 24, ' ', 0, 0x5);
    framebuffer_write(17, 25, ' ', 0, 0x8);
    framebuffer_write(17, 26, ' ', 0, 0x8);

    framebuffer_write(18, 1, ' ', 0, 0x8);//row 11
    framebuffer_write(18, 2, ' ', 0, 0x8);
    framebuffer_write(18, 3, ' ', 0, 0x8);
    framebuffer_write(18, 4, ' ', 0, 0x8);
    framebuffer_write(18, 5, ' ', 0, 0xF);
    framebuffer_write(18, 6, ' ', 0, 0xF);
    framebuffer_write(18, 7, ' ', 0, 0x8);
    framebuffer_write(18, 8, ' ', 0, 0x8);
    framebuffer_write(18, 9, ' ', 0, 0x8);
    framebuffer_write(18, 10, ' ', 0, 0x8);
    framebuffer_write(18, 11, ' ', 0, 0x5);
    framebuffer_write(18, 12, ' ', 0, 0x5);
    framebuffer_write(18, 13, ' ', 0, 0x8);
    framebuffer_write(18, 14, ' ', 0, 0x8);
    framebuffer_write(18, 15, ' ', 0, 0xD);
    framebuffer_write(18, 16, ' ', 0, 0xD);
    framebuffer_write(18, 17, ' ', 0, 0xD);
    framebuffer_write(18, 18, ' ', 0, 0xD);
    framebuffer_write(18, 19, ' ', 0, 0xD);
    framebuffer_write(18, 20, ' ', 0, 0xD);
    framebuffer_write(18, 21, ' ', 0, 0xD);
    framebuffer_write(18, 22, ' ', 0, 0xD);
    framebuffer_write(18, 23, ' ', 0, 0x5);
    framebuffer_write(18, 24, ' ', 0, 0x5);
    framebuffer_write(18, 25, ' ', 0, 0x8);
    framebuffer_write(18, 26, ' ', 0, 0x8);
    framebuffer_write(18, 27, ' ', 0, 0x8);
    framebuffer_write(18, 28, ' ', 0, 0x8);
    framebuffer_write(18, 29, ' ', 0, 0xD);
    framebuffer_write(18, 30, ' ', 0, 0xD);

    framebuffer_write(19, 3, ' ', 0, 0x8);//row 12
    framebuffer_write(19, 4, ' ', 0, 0x8);
    framebuffer_write(19, 5, ' ', 0, 0x8);
    framebuffer_write(19, 6, ' ', 0, 0x8);
    framebuffer_write(19, 9, ' ', 0, 0x8);
    framebuffer_write(19, 10, ' ', 0, 0x8);
    framebuffer_write(19, 11, ' ', 0, 0x5);
    framebuffer_write(19, 12, ' ', 0, 0x5);
    framebuffer_write(19, 13, ' ', 0, 0x8);
    framebuffer_write(19, 14, ' ', 0, 0x8);
    framebuffer_write(19, 15, ' ', 0, 0xD);
    framebuffer_write(19, 16, ' ', 0, 0xD);
    framebuffer_write(19, 17, ' ', 0, 0xD);
    framebuffer_write(19, 18, ' ', 0, 0xD);
    framebuffer_write(19, 19, ' ', 0, 0xD);
    framebuffer_write(19, 20, ' ', 0, 0xD);
    framebuffer_write(19, 21, ' ', 0, 0xD);
    framebuffer_write(19, 22, ' ', 0, 0xD);
    framebuffer_write(19, 23, ' ', 0, 0x5);
    framebuffer_write(19, 24, ' ', 0, 0x5);
    framebuffer_write(19, 25, ' ', 0, 0x5);
    framebuffer_write(19, 26, ' ', 0, 0x5);
    framebuffer_write(19, 27, ' ', 0, 0x5);
    framebuffer_write(19, 28, ' ', 0, 0x5);
    framebuffer_write(19, 29, ' ', 0, 0xD);
    framebuffer_write(19, 30, ' ', 0, 0xD);

    framebuffer_write(20, 11, ' ', 0, 0x8);//row 13
    framebuffer_write(20, 12, ' ', 0, 0x8);
    framebuffer_write(20, 13, ' ', 0, 0xD);
    framebuffer_write(20, 14, ' ', 0, 0xD);
    framebuffer_write(20, 15, ' ', 0, 0xD);
    framebuffer_write(20, 16, ' ', 0, 0xD);
    framebuffer_write(20, 17, ' ', 0, 0xD);
    framebuffer_write(20, 18, ' ', 0, 0xD);
    framebuffer_write(20, 19, ' ', 0, 0xD);
    framebuffer_write(20, 20, ' ', 0, 0xD);
    framebuffer_write(20, 21, ' ', 0, 0x5);
    framebuffer_write(20, 22, ' ', 0, 0x5);
    framebuffer_write(20, 23, ' ', 0, 0x5);
    framebuffer_write(20, 24, ' ', 0, 0x5);
    framebuffer_write(20, 25, ' ', 0, 0x5);
    framebuffer_write(20, 26, ' ', 0, 0x5);
    framebuffer_write(20, 27, ' ', 0, 0x5);
    framebuffer_write(20, 28, ' ', 0, 0x5);
    framebuffer_write(20, 29, ' ', 0, 0xD);
    framebuffer_write(20, 30, ' ', 0, 0xD);

    framebuffer_write(21, 13, ' ', 0, 0x8);//row 14
    framebuffer_write(21, 14, ' ', 0, 0x8);
    framebuffer_write(21, 15, ' ', 0, 0x8);
    framebuffer_write(21, 16, ' ', 0, 0x8);
    framebuffer_write(21, 17, ' ', 0, 0x8);
    framebuffer_write(21, 18, ' ', 0, 0x8);
    framebuffer_write(21, 19, ' ', 0, 0x8);
    framebuffer_write(21, 20, ' ', 0, 0x8);
    framebuffer_write(21, 21, ' ', 0, 0x8);
    framebuffer_write(21, 22, ' ', 0, 0x8);
    framebuffer_write(21, 23, ' ', 0, 0x8);
    framebuffer_write(21, 24, ' ', 0, 0x8);
    framebuffer_write(21, 25, ' ', 0, 0x8);
    framebuffer_write(21, 26, ' ', 0, 0x8);
    framebuffer_write(21, 27, ' ', 0, 0x8);
    framebuffer_write(21, 28, ' ', 0, 0x8);
    framebuffer_write(21, 29, ' ', 0, 0x8);
    framebuffer_write(21, 30, ' ', 0, 0x8);

    framebuffer_set_cursor(7, 13);

    while (TRUE);
}
