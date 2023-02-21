#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.h"
#include "lib-header/framebuffer.h"
#include "lib-header/kernel_loader.h"

void kernel_setup(void) {
    framebuffer_clear();
    framebuffer_write(2, 16+3, ' ', 0, 0x4); //row 2
    framebuffer_write(2, 16+4, ' ', 0, 0x4);
    framebuffer_write(2, 16+5, ' ', 0, 0x4);
    framebuffer_write(2, 16+6, ' ', 0, 0x4);
    framebuffer_write(2, 16+41, ' ', 0, 0x4);
    framebuffer_write(2, 16+42, ' ', 0, 0x4);
    framebuffer_write(2, 16+43, ' ', 0, 0x4);
    framebuffer_write(2, 16+44, ' ', 0, 0x4);

    framebuffer_write(3, 16+3, ' ', 0, 0x4); //row 3
    framebuffer_write(3, 16+4, ' ', 0, 0x4);
    framebuffer_write(3, 16+5, ' ', 0, 0x4);
    framebuffer_write(3, 16+6, ' ', 0, 0x4);
    framebuffer_write(3, 16+7, ' ', 0, 0x4);
    framebuffer_write(3, 16+8, ' ', 0, 0x4);
    framebuffer_write(3, 16+9, ' ', 0, 0x4);
    framebuffer_write(3, 16+10, ' ', 0, 0x4);
    framebuffer_write(3, 16+11, ' ', 0, 0x4);
    framebuffer_write(3, 16+12, ' ', 0, 0x4);
    framebuffer_write(3, 16+35, ' ', 0, 0x4);
    framebuffer_write(3, 16+36, ' ', 0, 0x4);
    framebuffer_write(3, 16+37, ' ', 0, 0x4);
    framebuffer_write(3, 16+38, ' ', 0, 0x4);
    framebuffer_write(3, 16+39, ' ', 0, 0x4);
    framebuffer_write(3, 16+40, ' ', 0, 0x4);
    framebuffer_write(3, 16+41, ' ', 0, 0x4);
    framebuffer_write(3, 16+42, ' ', 0, 0x4);
    framebuffer_write(3, 16+43, ' ', 0, 0x4);
    framebuffer_write(3, 16+44, ' ', 0, 0x4);

    framebuffer_write(4, 16+9, ' ', 0, 0x4); //row 4
    framebuffer_write(4, 16+10, ' ', 0, 0x4);
    framebuffer_write(4, 16+11, ' ', 0, 0x4);
    framebuffer_write(4, 16+12, ' ', 0, 0x4);
    framebuffer_write(4, 16+13, ' ', 0, 0x4);
    framebuffer_write(4, 16+14, ' ', 0, 0x4);
    framebuffer_write(4, 16+33, ' ', 0, 0x4);
    framebuffer_write(4, 16+34, ' ', 0, 0x4);
    framebuffer_write(4, 16+35, ' ', 0, 0x4);
    framebuffer_write(4, 16+36, ' ', 0, 0x4);
    framebuffer_write(4, 16+37, ' ', 0, 0x4);
    framebuffer_write(4, 16+38, ' ', 0, 0x4);

    framebuffer_write(5, 16+13, ' ', 0, 0x4); //row 5
    framebuffer_write(5, 16+14, ' ', 0, 0x4);
    framebuffer_write(5, 16+15, ' ', 0, 0x4);
    framebuffer_write(5, 16+16, ' ', 0, 0x4);
    framebuffer_write(5, 16+17, ' ', 0, 0x4);
    framebuffer_write(5, 16+18, ' ', 0, 0x4);
    framebuffer_write(5, 16+29, ' ', 0, 0x4);
    framebuffer_write(5, 16+30, ' ', 0, 0x4);
    framebuffer_write(5, 16+31, ' ', 0, 0x4);  
    framebuffer_write(5, 16+32, ' ', 0, 0x4);
    framebuffer_write(5, 16+33, ' ', 0, 0x4);
    framebuffer_write(5, 16+34, ' ', 0, 0x4);

    framebuffer_write(9, 16+1, ' ', 0, 0x8); //row 9
    framebuffer_write(9, 16+2, ' ', 0, 0x8);
    framebuffer_write(9, 16+3, ' ', 0, 0x8);
    framebuffer_write(9, 16+4, ' ', 0, 0x8);
    framebuffer_write(9, 16+43, ' ', 0, 0xF);
    framebuffer_write(9, 16+44, ' ', 0, 0xF);
    framebuffer_write(9, 16+45, ' ', 0, 0xF);
    framebuffer_write(9, 16+46, ' ', 0, 0xF);

    framebuffer_write(10, 16+3, ' ', 0, 0x8); //row 10
    framebuffer_write(10, 16+4, ' ', 0, 0x8);
    framebuffer_write(10, 16+7, ' ', 0, 0x8);
    framebuffer_write(10, 16+8, ' ', 0, 0x8);
    framebuffer_write(10, 16+39, ' ', 0, 0xF);
    framebuffer_write(10, 16+40, ' ', 0, 0xF);
    framebuffer_write(10, 16+43, ' ', 0, 0xF);
    framebuffer_write(10, 16+44, ' ', 0, 0xF);

    framebuffer_write(11, 16+7, ' ', 0, 0x8); //row 11
    framebuffer_write(11, 16+8, ' ', 0, 0x8);
    framebuffer_write(11, 16+9, ' ', 0, 0x8);
    framebuffer_write(11, 16+10, ' ', 0, 0x8);
    framebuffer_write(11, 16+13, ' ', 0, 0x8);
    framebuffer_write(11, 16+14, ' ', 0, 0x8);
    framebuffer_write(11, 16+17, ' ', 0, 0x8);
    framebuffer_write(11, 16+18, ' ', 0, 0x8);
    framebuffer_write(11, 16+21, ' ', 0, 0x8);
    framebuffer_write(11, 16+22, ' ', 0, 0x8);
    framebuffer_write(11, 16+23, ' ', 0, 0xF);
    framebuffer_write(11, 16+24, ' ', 0, 0xF);
    framebuffer_write(11, 16+25, ' ', 0, 0x8);
    framebuffer_write(11, 16+26, ' ', 0, 0x8);
    framebuffer_write(11, 16+29, ' ', 0, 0x8);
    framebuffer_write(11, 16+30, ' ', 0, 0x8);
    framebuffer_write(11, 16+31, ' ', 0, 0xF);
    framebuffer_write(11, 16+32, ' ', 0, 0xF);
    framebuffer_write(11, 16+33, ' ', 0, 0xF);
    framebuffer_write(11, 16+34, ' ', 0, 0xF);
    framebuffer_write(11, 16+37, ' ', 0, 0x8);
    framebuffer_write(11, 16+38, ' ', 0, 0x8);
    framebuffer_write(11, 16+39, ' ', 0, 0xF);
    framebuffer_write(11, 16+40, ' ', 0, 0xF);
    framebuffer_write(15, 16+13,' ', 0, 0x8);//row 15
    framebuffer_write(15, 16+14,' ', 0, 0x8);
    framebuffer_write(15, 16+17,' ', 0, 0x8);
    framebuffer_write(15, 16+18,' ', 0, 0x8);
    framebuffer_write(15, 16+19,' ', 0, 0xF);
    framebuffer_write(15, 16+20,' ', 0, 0xF);
    framebuffer_write(15, 16+21,' ', 0, 0x8);
    framebuffer_write(15, 16+22,' ', 0, 0x8);
    framebuffer_write(15, 16+25,' ', 0, 0x8);
    framebuffer_write(15, 16+26,' ', 0, 0x8);
    framebuffer_write(15, 16+27,' ', 0, 0xF);
    framebuffer_write(15, 16+28,' ', 0, 0xF);
    framebuffer_write(15, 16+29,' ', 0, 0xF);
    framebuffer_write(15, 16+30,' ', 0, 0xF);
    framebuffer_write(15, 16+33,' ', 0, 0xF);
    framebuffer_write(15, 16+34,' ', 0, 0xF);

    framebuffer_write(14, 16+11,' ', 0, 0x8);//row 14
    framebuffer_write(14, 16+12,' ', 0, 0x8);
    framebuffer_write(14, 16+13,' ', 0, 0x8);
    framebuffer_write(14, 16+14,' ', 0, 0x8);
    framebuffer_write(14, 16+17,' ', 0, 0x8);
    framebuffer_write(14, 16+18,' ', 0, 0x8);
    framebuffer_write(14, 16+19,' ', 0, 0x8);
    framebuffer_write(14, 16+20,' ', 0, 0x8);
    framebuffer_write(14, 16+21,' ', 0, 0x8);
    framebuffer_write(14, 16+22,' ', 0, 0x8);
    framebuffer_write(14, 16+25,' ', 0, 0x8);
    framebuffer_write(14, 16+26,' ', 0, 0x8);
    framebuffer_write(14, 16+27,' ', 0, 0xF);
    framebuffer_write(14, 16+28,' ', 0, 0xF);
    framebuffer_write(14, 16+29,' ', 0, 0xF);
    framebuffer_write(14, 16+30,' ', 0, 0xF);
    framebuffer_write(14, 16+33,' ', 0, 0x8);
    framebuffer_write(14, 16+34,' ', 0, 0x8);
    framebuffer_write(14, 16+35,' ', 0, 0xF);
    framebuffer_write(14, 16+36,' ', 0, 0xF);
    
    framebuffer_write(13, 16+11,' ', 0, 0x8);//row 13
    framebuffer_write(13, 16+12,' ', 0, 0x8);
    framebuffer_write(13, 16+15,' ', 0, 0x8);
    framebuffer_write(13, 16+16,' ', 0, 0x8);
    framebuffer_write(13, 16+19,' ', 0, 0x8);
    framebuffer_write(13, 16+20,' ', 0, 0x8);
    framebuffer_write(13, 16+23,' ', 0, 0xF);
    framebuffer_write(13, 16+24,' ', 0, 0xF);
    framebuffer_write(13, 16+27,' ', 0, 0x8);
    framebuffer_write(13, 16+28,' ', 0, 0x8);
    framebuffer_write(13, 16+31,' ', 0, 0xF);
    framebuffer_write(13, 16+32,' ', 0, 0xF);
    framebuffer_write(13, 16+35,' ', 0, 0x8);
    framebuffer_write(13, 16+36,' ', 0, 0x8);

    framebuffer_write(12, 16+7,' ', 0, 0xF);//row 12
    framebuffer_write(12, 16+8,' ', 0, 0xF);
    framebuffer_write(12, 16+9,' ', 0, 0x8);
    framebuffer_write(12, 16+10,' ', 0, 0x8);
    framebuffer_write(12, 16+13,' ', 0, 0x8);
    framebuffer_write(12, 16+14,' ', 0, 0x8);
    framebuffer_write(12, 16+15,' ', 0, 0x8);
    framebuffer_write(12, 16+16,' ', 0, 0x8);
    framebuffer_write(12, 16+17,' ', 0, 0x8);
    framebuffer_write(12, 16+18,' ', 0, 0x8);
    framebuffer_write(12, 16+21,' ', 0, 0x8);
    framebuffer_write(12, 16+22,' ', 0, 0x8);
    framebuffer_write(12, 16+23,' ', 0, 0xF);
    framebuffer_write(12, 16+24,' ', 0, 0xF);
    framebuffer_write(12, 16+25,' ', 0, 0x8);
    framebuffer_write(12, 16+26,' ', 0, 0x8);
    framebuffer_write(12, 16+29,' ', 0, 0x8);
    framebuffer_write(12, 16+30,' ', 0, 0x8);
    framebuffer_write(12, 16+31,' ', 0, 0xF);
    framebuffer_write(12, 16+32,' ', 0, 0xF);
    framebuffer_write(12, 16+33,' ', 0, 0xF);
    framebuffer_write(12, 16+34,' ', 0, 0xF);
    framebuffer_write(12, 16+37,' ', 0, 0x8);
    framebuffer_write(12, 16+38,' ', 0, 0x8);
    framebuffer_write(12, 16+39,' ', 0, 0xF);
    framebuffer_write(12, 16+40,' ', 0, 0xF);


    framebuffer_set_cursor(7, 13);

    while (TRUE);
}
