#include "isr.h"
#include "drivers/low_level.h"
#include "drivers/screen.h"
#include "drivers/serial.h"
#include "idt.h"
#include "kernel/types.h"
#include "kernel/utils.h"

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

void isr_handler(registers_t r) {
  serial_debug("received interrupt: ");
  char s[3];
  int_to_ascii(r.int_no, s);
  serial_puts(s);
  serial_puts(" -> ");
  serial_puts(exception_messages[r.int_no]);
  serial_puts("\n");
}

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8 n, isr_t handler) {
  interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
  if (r.int_no >= 40)
    port_byte_out(0xA0, 0x20); /* slave */
  port_byte_out(0x20, 0x20);   /* master */

  if (interrupt_handlers[r.int_no] != 0) {
    isr_t handler = interrupt_handlers[r.int_no];
    handler(r);
  }
}
