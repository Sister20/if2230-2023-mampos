#include "interrupt.h"
#include "../filesystem/fat32.h"

struct TSSEntry _interrupt_tss_entry = {
    .ss0 = GDT_KERNEL_DATA_SEGMENT_SELECTOR,
};

// Activate PIC mask for keyboard only
void activate_keyboard_interrupt(void)
{
    out(PIC1_DATA, PIC_DISABLE_ALL_MASK ^ (1 << IRQ_KEYBOARD));
    out(PIC2_DATA, PIC_DISABLE_ALL_MASK);
}

void io_wait(void)
{
    out(0x80, 0);
}

void pic_ack(uint8_t irq)
{
    if (irq >= 8)
        out(PIC2_COMMAND, PIC_ACK);
    out(PIC1_COMMAND, PIC_ACK);
}

void pic_remap(void)
{
    uint8_t a1, a2;

    // Save masks
    a1 = in(PIC1_DATA);
    a2 = in(PIC2_DATA);

    // Starts the initialization sequence in cascade mode
    out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    out(PIC1_DATA, PIC1_OFFSET); // ICW2: Master PIC vector offset
    io_wait();
    out(PIC2_DATA, PIC2_OFFSET); // ICW2: Slave PIC vector offset
    io_wait();
    out(PIC1_DATA, 0b0100); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    out(PIC2_DATA, 0b0010); // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    out(PIC1_DATA, ICW4_8086);
    io_wait();
    out(PIC2_DATA, ICW4_8086);
    io_wait();

    // Restore masks
    out(PIC1_DATA, a1);
    out(PIC2_DATA, a2);
}

void main_interrupt_handler(struct CPURegister cpu, uint32_t int_number, struct InterruptStack info)
{
    switch (int_number)
    {
    case PAGE_FAULT:
        __asm__("hlt");
        break;
    case PIC1_OFFSET + IRQ_KEYBOARD:
        keyboard_isr();
        break;
    case 0x30:
        syscall(cpu, info);
        break;
    default:
        break;
    }
    
}

void set_tss_kernel_current_stack(void)
{
    uint32_t stack_ptr;
    // Reading base stack frame instead esp
    __asm__ volatile("mov %%ebp, %0"
                     : "=r"(stack_ptr)
                     : /* <Empty> */);
    // Add 8 because 4 for ret address and other 4 is for stack_ptr variable
    _interrupt_tss_entry.esp0 = stack_ptr + 8;
}

void syscall(struct CPURegister cpu, __attribute__((unused)) struct InterruptStack info)
{
    if (cpu.eax == 0)
    {
        struct FAT32DriverRequest request = *(struct FAT32DriverRequest *)cpu.ebx;
        *((int8_t *)cpu.ecx) = read(request);
    }
    else if (cpu.eax == 4)
    {
        keyboard_state_activate();
        __asm__("sti"); // Due IRQ is disabled when main_interrupt_handler() called
        while (is_keyboard_blocking())
            ;
        char buf[KEYBOARD_BUFFER_SIZE];
        get_keyboard_buffer(buf);
        memcpy((char *)cpu.ebx, buf, cpu.ecx);
    }
    else if (cpu.eax == 5)
    {
        uint8_t row, col;
        framebuffer_get_cursor(&row, &col);
        puts((char *)cpu.ebx, cpu.ecx, cpu.edx, row, col); // Modified puts() on kernel side
    }
}

void puts(char *str, uint32_t len, uint8_t color, uint8_t row, uint8_t col) {
    framebuffer_set_cursor(row, col);
    for (uint32_t i = 0; i < len; i++) {
        framebuffer_write(0, i + col, str[i], color, 0);
    }
    framebuffer_set_cursor(row, col + len);
}

