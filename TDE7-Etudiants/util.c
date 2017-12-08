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

#include "util.h"
#if 0
uint8_t pos = 0x0;

void plus_plus_handler(void) {
      uint8_t val = 1 << (pos);
			pos = (pos+1)%4;
			val = val & 0x7;
      write_f4_leds (val);
}
#endif
void tim4_setup(uint32_t prescaler,uint32_t period) {
   // Output sqaure signal on PD12 green led (TIM4_ch1) + irq

   nvic_enable_irq(NVIC_TIM4_IRQ);

   // Mode no input freq divider, edge counting up
   // CR1  CKD = 00  CMS=00 DIR=0
   timer_set_mode(TIM4,TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

   // Prescaler to 1Khz : div 420000 (apb1 = 42 Mhz) PSC register
   timer_set_prescaler(TIM4, prescaler);

   // Repeat counting  CR1 OPM = 0 
   timer_continuous_mode(TIM4);

   timer_set_period(TIM4,period);

   // enable_irq : output compare
   timer_enable_irq(TIM4, TIM_DIER_CC1DE);

   // enable output compare, active high
   timer_set_oc_polarity_high(TIM4,TIM_OC1);
   timer_enable_oc_output(TIM4,TIM_OC1);

   // Configure green led pin as TIM4ch2 output
   configure_pin(GREEN_0W_LED_TIM1CH2);

   // pwm mode
   timer_enable_oc_preload(TIM4,TIM_OC1);     // OcxPE=1
   timer_set_oc_mode(TIM4,TIM_OC1 ,TIM_OCM_PWM2);
   timer_set_oc_value(TIM4,TIM_OC1,period/2);
   timer_generate_event(TIM4,1);              // force UG bit

   // start timer
   timer_enable_counter(TIM4);
}

// Prescaler : divide 42MHz apb1 freq to timer input
void tim2_setup(uint32_t prescaler,uint32_t period) {
   nvic_enable_irq(NVIC_TIM2_IRQ);

   // Mode no input freq divider, edge counting up
   // CR1  CKD = 00  CMS=00 DIR=0
   timer_set_mode(TIM2,TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

   // Prescaler to 1Khz : div 420000 (apb1 = 42 Mhz) PSC register
   timer_set_prescaler(TIM2, prescaler);

   // Repeat counting  CR1 OPM = 0 
   timer_continuous_mode(TIM2);

   timer_set_period(TIM2,period)  ;

   // enable_irq : output compare ch3
   timer_enable_irq(TIM2, TIM_DIER_CC3IE);

   // enable output compare, active high
   timer_set_oc_polarity_high(TIM2,TIM_OC3);
   timer_enable_oc_output(TIM2,TIM_OC3);

   // pwm mode
   timer_enable_oc_preload(TIM2,TIM_OC3);     // OcxPE=1
   timer_set_oc_mode(TIM2,TIM_OC3 ,TIM_OCM_PWM2);
   timer_set_oc_value(TIM2,TIM_OC3,period/2);
   timer_generate_event(TIM2,1);              // force UG bit

   // start timer
   timer_enable_counter(TIM2);
}

void exti0_setup (void) {
   exti_set_trigger(EXTI0,EXTI_TRIGGER_RISING);     // IRQ on Px0 rising edge
   exti_select_source(EXTI0,GPIOA);                 // pa0 : user BP
   exti_enable_request(EXTI0);                      // irq + event
}

void do_test_exti0 (void) {
  exti0_setup ();
  nvic_enable_irq (NVIC_EXTI0_IRQ);
}

void util_setup(void) {
   // led green on TIM4_CH2 output  0,25Hz
   // 42Mhz input -> prescaler 1kHz -> freq 1/4 Hz

   tim4_setup (42000,1000);

   // counter on other leds, TiIM2 CH3 irq, 1Hz
   // leds counter incremented by irq
   // 42Mhz input -> prescaler 1kHz -> freq 1 Hz

   tim2_setup(42000,1000);
}
