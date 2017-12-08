#include "cold_startup.h"
#include "usb_configuration.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>


void test_lire_n () {
   unsigned int lu;

   while(1) {
     lu = read_n();
   }
}

void test_shield_leds () {
     write_led (P1_DRIVE,1);
     write_led (P1_DRIVE,0);
     write_led (P2_DRIVE,1);
     write_led (P2_DRIVE,0);
}
  
   
void test_rst_power () {
     write_led (P1_DRIVE,1);
     set_rst_pin (P1_DRIVE,0);
     set_rst_pin (P1_DRIVE,1);
     set_pwr_pin (P1_DRIVE,0);
     set_pwr_pin (P1_DRIVE,1);
     write_led (P1_DRIVE,0);
     write_led (P2_DRIVE,1);
     set_rst_pin (P2_DRIVE,0);
     set_rst_pin (P2_DRIVE,1);
     set_pwr_pin (P2_DRIVE,0);
     set_pwr_pin (P2_DRIVE,1);
     write_led (P2_DRIVE,0);
}

void test_pulse_trigger () {
     set_trigger(0);
     set_trigger(1);
     timer_set_frequency(42000,2500);        // 1Khz prescaler, 2,5sec
     timer_start();
}

void tim3_isr (void) {
  static bool v = true;
  timer_clear_flag(TIM3,TIM_SR_UIF);
  nvic_clear_pending_irq(NVIC_TIM3_IRQ);
  write_led(P1_DRIVE,v);
//  set_timer_pulse(v);
  v = v ? 0 : 1;
}

int main (void) {
  uint32_t lu;

   cold_startup();

//   gpio_port_write(GPIOD,0xFFFF);

//   usb_init ();

/*
   plusplus();
   
   while (1)
   {
        usbd_poll(usbd_dev);
   }
*/

//   test_lire_n();
//   test_shield_leds ();  
//   test_rst_power();
   test_pulse_trigger ();  

    while (1) {
       lu = *(uint32_t *) 0x40000418;
    }
   return 0;
}
