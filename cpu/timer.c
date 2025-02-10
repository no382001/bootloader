#include "timer.h"
#include "drivers/low_level.h"
#include "drivers/screen.h"
#include "isr.h"
#include "kernel/utils.h"

u32 tick = 0;

static void timer_callback(registers_t regs) {
  tick++;
  kernel_puts("tick: ");

  char tick_ascii[256];
  int_to_ascii(tick, tick_ascii);
  kernel_puts(tick_ascii);
  kernel_puts("\n");
}

void init_timer(u32 freq) {
  register_interrupt_handler(IRQ0, timer_callback);

  /* get PIT value: hardware clock at 1193180 Hz */
  u32 divisor = 1193180 / freq;
  u8 low = (u8)(divisor & 0xFF);
  u8 high = (u8)((divisor >> 8) & 0xFF);

  port_byte_out(0x43, 0x36); /* command port */
  port_byte_out(0x40, low);
  port_byte_out(0x40, high);
}
