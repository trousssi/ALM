/*
 *
 *  Copyright (C) 2017 Philippe Waille <Philippe.Waille@univ-grenoble-alpes.fr>
 *
 *  This discovery examples code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this code.  If not, see <http://www.gnu.org/licenses/>.
 *
*/


#include "cold_startup.h"
#include "spi_mems.h"
#include "discoveryf4_wiring.h"

// lib cmsis : nvic irq and systoick management

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

// libopencm3 : peripherals

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/syscfg.h>    // for exti lines configuration



uint16_t syscount = 0;
uint8_t val = 0x0;

void plus_plus_handler() {
      val = (val +1) & 0xF;
      write_f4_leds (val);
}

uint32_t sysms = 0;

void sys_tick_handler () {
   syscount++;
   if (syscount == 500) {
      syscount = 0;
//      plus_plus_handler();
   }
   if (sysms >0) sysms --;
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

static void exti0_setup () {
   exti_select_source(EXTI0,GPIOE);                     // pe0 : mems LIS
   exti_set_trigger(EXTI0,EXTI_TRIGGER_RISING);     // IRQ on Px0 rising edge
   exti_enable_request(EXTI0);                      // irq + event
}

int main (void) {

   coldstart();
   write_f4_leds(0);
   systick_setup();

   mems_init(); 

   exti0_setup();
   nvic_enable_irq (NVIC_EXTI0_IRQ);

  while(1) {
     mems_display_sample();
  }

   return 0;
}
