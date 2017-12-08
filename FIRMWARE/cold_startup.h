#ifndef _COLDSTART_H
#define _COLDSTART_H

#include <libopencm3/stm32/timer.h>



typedef enum {P2_DRIVE=1, P1_DRIVE=0} drive_t;

extern uint8_t read_n();

extern void write_leds(uint8_t value);

extern void set_rst_pin (drive_t drive, bool value);
extern void set_pwr_pin (drive_t drive, bool value);

extern void set_timer_pulse(bool v);

extern void timer_set_frequency(uint16_t prescaler, uint16_t period);

static inline void timer_start() { timer_enable_counter  (TIM3); } // CR1 Counter Enable bit = 1 
static inline void timer_stop()  { timer_disable_counter (TIM3); } // CR1 Counter Enable bit = 0 

void coldstart();
void plusplus ();


#endif
