#include "bits.h"

void isr_install() {
  set_idt_gate(0, (u32)isr0);
  set_idt_gate(1, (u32)isr1);
  set_idt_gate(2, (u32)isr2);
  set_idt_gate(3, (u32)isr3);
  set_idt_gate(4, (u32)isr4);
  set_idt_gate(5, (u32)isr5);
  set_idt_gate(6, (u32)isr6);
  set_idt_gate(7, (u32)isr7);
  set_idt_gate(8, (u32)isr8);
  set_idt_gate(9, (u32)isr9);
  set_idt_gate(10, (u32)isr10);
  set_idt_gate(11, (u32)isr11);
  set_idt_gate(12, (u32)isr12);
  set_idt_gate(13, (u32)isr13);
  set_idt_gate(14, (u32)isr14);
  set_idt_gate(15, (u32)isr15);
  set_idt_gate(16, (u32)isr16);
  set_idt_gate(17, (u32)isr17);
  set_idt_gate(18, (u32)isr18);
  set_idt_gate(19, (u32)isr19);
  set_idt_gate(20, (u32)isr20);
  set_idt_gate(21, (u32)isr21);
  set_idt_gate(22, (u32)isr22);
  set_idt_gate(23, (u32)isr23);
  set_idt_gate(24, (u32)isr24);
  set_idt_gate(25, (u32)isr25);
  set_idt_gate(26, (u32)isr26);
  set_idt_gate(27, (u32)isr27);
  set_idt_gate(28, (u32)isr28);
  set_idt_gate(29, (u32)isr29);
  set_idt_gate(30, (u32)isr30);
  set_idt_gate(31, (u32)isr31);

  // remap pic https://wiki.osdev.org/8259_PIC
  port_byte_out(0x20, 0x11);
  port_byte_out(0xA0, 0x11);
  port_byte_out(0x21, 0x20);
  port_byte_out(0xA1, 0x28);
  port_byte_out(0x21, 0x04);
  port_byte_out(0xA1, 0x02);
  port_byte_out(0x21, 0x01);
  port_byte_out(0xA1, 0x01);
  port_byte_out(0x21, 0x0);
  port_byte_out(0xA1, 0x0);

  set_idt_gate(32, (u32)irq0);
  set_idt_gate(33, (u32)irq1);
  set_idt_gate(34, (u32)irq2);
  set_idt_gate(35, (u32)irq3);
  set_idt_gate(36, (u32)irq4);
  set_idt_gate(37, (u32)irq5);
  set_idt_gate(38, (u32)irq6);
  set_idt_gate(39, (u32)irq7);
  set_idt_gate(40, (u32)irq8);
  set_idt_gate(41, (u32)irq9);
  set_idt_gate(42, (u32)irq10);
  set_idt_gate(43, (u32)irq11);
  set_idt_gate(44, (u32)irq12);
  set_idt_gate(45, (u32)irq13);
  set_idt_gate(46, (u32)irq14);
  set_idt_gate(47, (u32)irq15);

  set_idt(); //
}

static char *exception_messages[] = {"division by zero",
                                     "debug",
                                     "non maskable interrupt",
                                     "breakpoint",
                                     "into detected overflow",
                                     "out of bounds",
                                     "invalid opcode",
                                     "no coprocessor",

                                     "double fault",
                                     "coprocessor segment overrun",
                                     "bad tss",
                                     "segment not present",
                                     "stack fault",
                                     "general protection fault",
                                     "page fault",
                                     "unknown interrupt",

                                     "coprocessor fault",
                                     "alignment check",
                                     "machine check",
                                     "reserved",
                                     "reserved",
                                     "reserved",
                                     "reserved",
                                     "reserved",

                                     "reserved",
                                     "reserved",
                                     "reserved",
                                     "reserved",
                                     "reserved",
                                     "reserved",
                                     "reserved",
                                     "reserved"};

void isr_handler(registers_t *r) {
  serial_debug("received interrupt: ");
  char s[3];
  int_to_ascii(r->int_no, s);
  serial_puts(s);
  serial_puts(" -> ");
  serial_puts(exception_messages[r->int_no]);
  serial_puts("\n");
  asm volatile("hlt");
}

isr_t interrupt_handlers[256] = {0};

void register_interrupt_handler(u8 n, isr_t handler) {
  interrupt_handlers[n] = handler;
}

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20 /* end-of-interrupt command code */

void irq_handler(registers_t *r) {
  if (r->int_no >= 40) // why 40?
    port_byte_out(PIC2, PIC_EOI);
  port_byte_out(PIC1, PIC_EOI);

  if (interrupt_handlers[r->int_no] != 0) {
    isr_t handler = interrupt_handlers[r->int_no];
    handler(r);
  }
}

void irq_install() {
  asm volatile("sti");
  init_timer(50);
  init_keyboard();
}
