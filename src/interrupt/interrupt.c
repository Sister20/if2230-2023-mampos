#include "interrupt.h"


// Activate PIC mask for keyboard only
void activate_keyboard_interrupt(void)
{
    uint8_t keyboard_irq = PIC1_OFFSET + IRQ_KEYBOARD;
    uint8_t mask = inb(PIC1_DATA) & ~(1 << IRQ_KEYBOARD);
    outb(PIC1_DATA, mask);
}

// I/O port wait, around 1-4 microseconds, for I/O synchronization putpose
void io_wait(void) {
    out(0x80, 0);
}

// Send ACK to PIC - @param irq Interrupt request number destination, note: this function already include PIC1_OFFSET
void pic_ack(uint8_t irq)
{
    if (irq >= PIC1_OFFSET) {
        outb(PIC2_COMMAND, PIC_ACK);
    }
    outb(PIC1_COMMAND, PIC_ACK);
}

// Shift PIC interrupt number to PIC1_OFFSET and PIC2_OFFSET (master and slave)
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
    out(PIC1_DATA, 0b0100);      // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    out(PIC2_DATA, 0b0010);      // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    out(PIC1_DATA, ICW4_8086);
    io_wait();
    out(PIC2_DATA, ICW4_8086);
    io_wait();

    // Restore masks
    out(PIC1_DATA, a1);
    out(PIC2_DATA, a2);

}

void main_interrupt_handler(struct CPURegister cpu, uint32_t int_number, struct InterruptStack *frame)
{
    // Call respective ISR based on interrupt number
    switch (int_number) {
        case IRQ_TIMER:
            timer_interrupt_handler(cpu, frame);
            break;
        case IRQ_KEYBOARD:
            keyboard_interrupt_handler(cpu, frame);
            break;
        case IRQ_MOUSE:
            mouse_interrupt_handler(cpu, frame);
            break;
    }
}