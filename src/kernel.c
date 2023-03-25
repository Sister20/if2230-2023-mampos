#include "lib-header/portio.h"
#include "lib-header/stdtype.h"
#include "lib-header/stdmem.h"
#include "lib-header/gdt.c"
#include "lib-header/framebuffer.h"
#include "lib-header/kernel_loader.h"
#include "interrupt/idt.c"
#include "interrupt/interrupt.c"

void kernel_setup(void)
{
    enter_protected_mode(&_gdt_gdtr);
    pic_remap();
    initialize_idt();
    framebuffer_clear();
    framebuffer_set_cursor(20, 45);
    __asm__("int $0x4");
    while (TRUE)
        ;

    // framebuffer_write(3, 8,  'H', 0, 0xF);
    // framebuffer_write(3, 9,  'a', 0, 0xF);
    // framebuffer_write(3, 10, 'i', 0, 0xF);
    // framebuffer_write(3, 11, '!', 0, 0xF);

    // framebuffer_write(2, 16+3, ' ', 0, 0x4); //row 2
    // framebuffer_write(2, 16+4, ' ', 0, 0x4);
    // framebuffer_write(2, 16+5, ' ', 0, 0x4);
    // framebuffer_write(2, 16+6, ' ', 0, 0x4);
    // framebuffer_write(2, 16+41, ' ', 0, 0x4);
    // framebuffer_write(2, 16+42, ' ', 0, 0x4);
    // framebuffer_write(2, 16+43, ' ', 0, 0x4);
    // framebuffer_write(2, 16+44, ' ', 0, 0x4);

    // framebuffer_write(3, 16+3, ' ', 0, 0x4); //row 3
    // framebuffer_write(3, 16+4, ' ', 0, 0x4);
    // framebuffer_write(3, 16+5, ' ', 0, 0x4);
    // framebuffer_write(3, 16+6, ' ', 0, 0x4);
    // framebuffer_write(3, 16+7, ' ', 0, 0x4);
    // framebuffer_write(3, 16+8, ' ', 0, 0x4);
    // framebuffer_write(3, 16+9, ' ', 0, 0x4);
    // framebuffer_write(3, 16+10, ' ', 0, 0x4);
    // framebuffer_write(3, 16+11, ' ', 0, 0x4);
    // framebuffer_write(3, 16+12, ' ', 0, 0x4);
    // framebuffer_write(3, 16+35, ' ', 0, 0x4);
    // framebuffer_write(3, 16+36, ' ', 0, 0x4);
    // framebuffer_write(3, 16+37, ' ', 0, 0x4);
    // framebuffer_write(3, 16+38, ' ', 0, 0x4);
    // framebuffer_write(3, 16+39, ' ', 0, 0x4);
    // framebuffer_write(3, 16+40, ' ', 0, 0x4);
    // framebuffer_write(3, 16+41, ' ', 0, 0x4);
    // framebuffer_write(3, 16+42, ' ', 0, 0x4);
    // framebuffer_write(3, 16+43, ' ', 0, 0x4);
    // framebuffer_write(3, 16+44, ' ', 0, 0x4);

    // framebuffer_write(4, 16+9, ' ', 0, 0x4); //row 4
    // framebuffer_write(4, 16+10, ' ', 0, 0x4);
    // framebuffer_write(4, 16+11, ' ', 0, 0x4);
    // framebuffer_write(4, 16+12, ' ', 0, 0x4);
    // framebuffer_write(4, 16+13, ' ', 0, 0x4);
    // framebuffer_write(4, 16+14, ' ', 0, 0x4);
    // framebuffer_write(4, 16+33, ' ', 0, 0x4);
    // framebuffer_write(4, 16+34, ' ', 0, 0x4);
    // framebuffer_write(4, 16+35, ' ', 0, 0x4);
    // framebuffer_write(4, 16+36, ' ', 0, 0x4);
    // framebuffer_write(4, 16+37, ' ', 0, 0x4);
    // framebuffer_write(4, 16+38, ' ', 0, 0x4);

    // framebuffer_write(5, 16+13, ' ', 0, 0x4); //row 5
    // framebuffer_write(5, 16+14, ' ', 0, 0x4);
    // framebuffer_write(5, 16+15, ' ', 0, 0x4);
    // framebuffer_write(5, 16+16, ' ', 0, 0x4);
    // framebuffer_write(5, 16+17, ' ', 0, 0x4);
    // framebuffer_write(5, 16+18, ' ', 0, 0x4);
    // framebuffer_write(5, 16+29, ' ', 0, 0x4);
    // framebuffer_write(5, 16+30, ' ', 0, 0x4);
    // framebuffer_write(5, 16+31, ' ', 0, 0x4);
    // framebuffer_write(5, 16+32, ' ', 0, 0x4);
    // framebuffer_write(5, 16+33, ' ', 0, 0x4);
    // framebuffer_write(5, 16+34, ' ', 0, 0x4);

    // framebuffer_write(9, 16+1, ' ', 0, 0x8); //row 9
    // framebuffer_write(9, 16+2, ' ', 0, 0x8);
    // // framebuffer_write(9, 16+3, ' ', 0, 0x8);
    // // framebuffer_write(9, 16+4, ' ', 0, 0x8);
    // // framebuffer_write(9, 16+43, ' ', 0, 0xF);
    // // framebuffer_write(9, 16+44, ' ', 0, 0xF);
    // framebuffer_write(9, 16+45, ' ', 0, 0xF);
    // framebuffer_write(9, 16+46, ' ', 0, 0xF);

    // framebuffer_write(10, 16+3, ' ', 0, 0x8); //row 10
    // framebuffer_write(10, 16+4, ' ', 0, 0x8);
    // framebuffer_write(10, 16+7, ' ', 0, 0x8);
    // framebuffer_write(10, 16+8, ' ', 0, 0x8);
    // framebuffer_write(10, 16+39, ' ', 0, 0xF);
    // framebuffer_write(10, 16+40, ' ', 0, 0xF);
    // framebuffer_write(10, 16+43, ' ', 0, 0xF);
    // framebuffer_write(10, 16+44, ' ', 0, 0xF);

    // framebuffer_write(11, 16+7, ' ', 0, 0x8); //row 11
    // framebuffer_write(11, 16+8, ' ', 0, 0x8);
    // framebuffer_write(11, 16+9, ' ', 0, 0x8);
    // framebuffer_write(11, 16+10, ' ', 0, 0x8);
    // framebuffer_write(11, 16+13, ' ', 0, 0x8);
    // framebuffer_write(11, 16+14, ' ', 0, 0x8);
    // framebuffer_write(11, 16+17, ' ', 0, 0x8);
    // framebuffer_write(11, 16+18, ' ', 0, 0x8);
    // framebuffer_write(11, 16+21, ' ', 0, 0x8);
    // framebuffer_write(11, 16+22, ' ', 0, 0x8);
    // framebuffer_write(11, 16+23, ' ', 0, 0xF);
    // framebuffer_write(11, 16+24, ' ', 0, 0xF);
    // framebuffer_write(11, 16+25, ' ', 0, 0x8);
    // framebuffer_write(11, 16+26, ' ', 0, 0x8);
    // framebuffer_write(11, 16+29, ' ', 0, 0x8);
    // framebuffer_write(11, 16+30, ' ', 0, 0x8);
    // framebuffer_write(11, 16+31, ' ', 0, 0xF);
    // framebuffer_write(11, 16+32, ' ', 0, 0xF);
    // framebuffer_write(11, 16+33, ' ', 0, 0xF);
    // framebuffer_write(11, 16+34, ' ', 0, 0xF);
    // framebuffer_write(11, 16+37, ' ', 0, 0x8);
    // framebuffer_write(11, 16+38, ' ', 0, 0x8);
    // framebuffer_write(11, 16+39, ' ', 0, 0xF);
    // framebuffer_write(11, 16+40, ' ', 0, 0xF);
    // framebuffer_write(15, 16+13,' ', 0, 0x8);//row 15
    // framebuffer_write(15, 16+14,' ', 0, 0x8);
    // framebuffer_write(15, 16+17,' ', 0, 0x8);
    // framebuffer_write(15, 16+18,' ', 0, 0x8);
    // framebuffer_write(15, 16+19,' ', 0, 0xF);
    // framebuffer_write(15, 16+20,' ', 0, 0xF);
    // framebuffer_write(15, 16+21,' ', 0, 0x8);
    // framebuffer_write(15, 16+22,' ', 0, 0x8);
    // framebuffer_write(15, 16+25,' ', 0, 0x8);
    // framebuffer_write(15, 16+26,' ', 0, 0x8);
    // framebuffer_write(15, 16+27,' ', 0, 0xF);
    // framebuffer_write(15, 16+28,' ', 0, 0xF);
    // framebuffer_write(15, 16+29,' ', 0, 0xF);
    // framebuffer_write(15, 16+30,' ', 0, 0xF);
    // framebuffer_write(15, 16+33,' ', 0, 0xF);
    // framebuffer_write(15, 16+34,' ', 0, 0xF);

    // framebuffer_write(14, 16+11,' ', 0, 0x8);//row 14
    // framebuffer_write(14, 16+12,' ', 0, 0x8);
    // framebuffer_write(14, 16+13,' ', 0, 0x8);
    // framebuffer_write(14, 16+14,' ', 0, 0x8);
    // framebuffer_write(14, 16+17,' ', 0, 0x8);
    // framebuffer_write(14, 16+18,' ', 0, 0x8);
    // framebuffer_write(14, 16+19,' ', 0, 0x8);
    // framebuffer_write(14, 16+20,' ', 0, 0x8);
    // framebuffer_write(14, 16+21,' ', 0, 0x8);
    // framebuffer_write(14, 16+22,' ', 0, 0x8);
    // framebuffer_write(14, 16+25,' ', 0, 0x8);
    // framebuffer_write(14, 16+26,' ', 0, 0x8);
    // framebuffer_write(14, 16+27,' ', 0, 0xF);
    // framebuffer_write(14, 16+28,' ', 0, 0xF);
    // framebuffer_write(14, 16+29,' ', 0, 0xF);
    // framebuffer_write(14, 16+30,' ', 0, 0xF);
    // framebuffer_write(14, 16+33,' ', 0, 0x8);
    // framebuffer_write(14, 16+34,' ', 0, 0x8);
    // framebuffer_write(14, 16+35,' ', 0, 0xF);
    // framebuffer_write(14, 16+36,' ', 0, 0xF);

    // framebuffer_write(13, 16+11,' ', 0, 0x8);//row 13
    // framebuffer_write(13, 16+12,' ', 0, 0x8);
    // framebuffer_write(13, 16+15,' ', 0, 0x8);
    // framebuffer_write(13, 16+16,' ', 0, 0x8);
    // framebuffer_write(13, 16+19,' ', 0, 0x8);
    // framebuffer_write(13, 16+20,' ', 0, 0x8);
    // framebuffer_write(13, 16+23,' ', 0, 0xF);
    // framebuffer_write(13, 16+24,' ', 0, 0xF);
    // framebuffer_write(13, 16+27,' ', 0, 0x8);
    // framebuffer_write(13, 16+28,' ', 0, 0x8);
    // framebuffer_write(13, 16+31,' ', 0, 0xF);
    // framebuffer_write(13, 16+32,' ', 0, 0xF);
    // framebuffer_write(13, 16+35,' ', 0, 0x8);
    // framebuffer_write(13, 16+36,' ', 0, 0x8);

    // framebuffer_write(12, 16+7,' ', 0, 0xF);//row 12
    // framebuffer_write(12, 16+8,' ', 0, 0xF);
    // framebuffer_write(12, 16+9,' ', 0, 0x8);
    // framebuffer_write(12, 16+10,' ', 0, 0x8);
    // framebuffer_write(12, 16+13,' ', 0, 0x8);
    // framebuffer_write(12, 16+14,' ', 0, 0x8);
    // framebuffer_write(12, 16+15,' ', 0, 0x8);
    // framebuffer_write(12, 16+16,' ', 0, 0x8);
    // framebuffer_write(12, 16+17,' ', 0, 0x8);
    // framebuffer_write(12, 16+18,' ', 0, 0x8);
    // framebuffer_write(12, 16+21,' ', 0, 0x8);
    // framebuffer_write(12, 16+22,' ', 0, 0x8);
    // framebuffer_write(12, 16+23,' ', 0, 0xF);
    // framebuffer_write(12, 16+24,' ', 0, 0xF);
    // framebuffer_write(12, 16+25,' ', 0, 0x8);
    // framebuffer_write(12, 16+26,' ', 0, 0x8);
    // framebuffer_write(12, 16+29,' ', 0, 0x8);
    // framebuffer_write(12, 16+30,' ', 0, 0x8);
    // framebuffer_write(12, 16+31,' ', 0, 0xF);
    // framebuffer_write(12, 16+32,' ', 0, 0xF);
    // framebuffer_write(12, 16+33,' ', 0, 0xF);
    // framebuffer_write(12, 16+34,' ', 0, 0xF);
    // framebuffer_write(12, 16+37,' ', 0, 0x8);
    // framebuffer_write(12, 16+38,' ', 0, 0x8);
    // framebuffer_write(12, 16+39,' ', 0, 0xF);
    // framebuffer_write(12, 16+40,' ', 0, 0xF);

    // // framebuffer_write(18, 34+1,'W', 0xF, 0);//row 15
    // // framebuffer_write(18, 34+2,'e', 0xF, 0);
    // // framebuffer_write(18, 34+3,'l', 0xF, 0);
    // // framebuffer_write(18, 34+4,'c', 0xF, 0);
    // // framebuffer_write(18, 34+5,'o', 0xF, 0);
    // // framebuffer_write(18, 34+6,'m', 0xF, 0);
    // // framebuffer_write(18, 34+7,'e', 0xF, 0);

    // // framebuffer_write(18, 34+9,'t', 0xF, 0);//row 16
    // // framebuffer_write(18, 34+10,'o', 0xF, 0);

    // framebuffer_write(20, 33+1,'m', 0xF, 0);//row 17
    // framebuffer_write(20, 33+3,'a', 0xF, 0);
    // framebuffer_write(20, 33+5,'m', 0xF, 0);
    // framebuffer_write(20, 33+7,'p', 0xF, 0);
    // framebuffer_write(20, 33+10,'O', 0x4, 0);
    // framebuffer_write(20, 33+12,'S', 0x4, 0);

    // // framebuffer_write(22, 12+1,'w', 0xF, 0);//row 17
    // // framebuffer_write(22, 12+2,'a', 0xF, 0);
    // // framebuffer_write(22, 12+3,'n', 0xF, 0);
    // // framebuffer_write(22, 12+4,'n', 0xF, 0);
    // // framebuffer_write(22, 12+5,'a', 0xF, 0);
    // // framebuffer_write(22, 12+7,'P', 0x4, 0);
    // // framebuffer_write(22, 12+8,'L', 0x4, 0);
    // // framebuffer_write(22, 12+9,'A', 0x4, 0);
    // // framebuffer_write(22, 12+10,'Y', 0x4, 0);
    // // framebuffer_write(22, 12+11,'?', 0x4, 0);

    // // framebuffer_write(23, 58+1,'l', 0xF, 0);//row 17
    // // framebuffer_write(23, 58+2,'e', 0xF, 0);
    // // framebuffer_write(23, 58+3,'t', 0xF, 0);
    // // framebuffer_write(23, 58+4,'s', 0xF, 0);
    // // framebuffer_write(23, 58+6,'P', 0x4, 0);
    // // framebuffer_write(23, 58+7,'L', 0x4, 0);
    // // framebuffer_write(23, 58+8,'A', 0x4, 0);
    // // framebuffer_write(23, 58+9,'Y', 0x4, 0);
    // // framebuffer_write(23, 58+10,'!', 0x4, 0);
}
