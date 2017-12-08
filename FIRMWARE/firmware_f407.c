#include "cold_startup.h"
#include "shield_discoveryf4_wiring.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>


#ifdef T
void tim3_isr (void) {
  static bool v = true;
  timer_clear_flag(TIM3,TIM_SR_UIF);
  nvic_clear_pending_irq(NVIC_TIM3_IRQ);
  write_led(P1_DRIVE,v);
//  set_timer_pulse(v);
  v = v ? 0 : 1;
}

#endif

uint16_t syscount = 0;
uint8_t un = 0x1;
uint8_t decl = 0x0;
uint8_t val = 0x0;
void plus_plus_handler() {
			decl = (decl +1) %4;
      val = (un  << decl) & 0xF;
      write_f4_leds (val);
}

void sys_tick_handler () {
   syscount++;
   if (syscount == 500) {
      syscount = 0;
      plus_plus_handler();
   }
}

static void systick_setup(void)
{
	/* clock rate / 1000 to get 1mS interrupt rate */
	systick_set_reload(168000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	/* this done last */
	systick_interrupt_enable();
}

void do_test_systick () {
   systick_setup();
}

void exti0_isr () {
  exti_reset_request(EXTI0);
  plus_plus_handler ();
}

static void exti0_setup () {
//   exti_select_source(0,GPIOE);   // pe0 : mems LIS

   exti_set_trigger(EXTI0,EXTI_TRIGGER_RISING);     // IRQ on Px0 rising edge
   exti_select_source(EXTI0,GPIOA);                 // pa0 : user BP
   exti_enable_request(EXTI0);                      // irq + event
}

void do_test_exti0 () {
  exti0_setup ();
  nvic_enable_irq (NVIC_EXTI0_IRQ);
}

int main (void) {
   int x;
   uint32_t adresse;

   coldstart();
   write_f4_leds(val);

   do_test_systick ();
   do_test_exti0 ();


   while (1) {
     x++;
   }

#ifdef BP
   while (1) {
     val = 0x5 ^ read_BP_USER();
     write_f4_leds((uint8_t) val);
   }
#endif
       
   return 0;
}
