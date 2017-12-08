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


#ifndef DISCOVERYF4_WIRING_H
#define DISCOVERYF4_WIRING_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>

typedef const struct {
  uint32_t port;
  uint16_t pin;
  uint8_t mode;
  uint8_t pullup;
  uint8_t afnum;     
} portpin_t;

// Dummy alternate function number for input, output and analog pins
#define GPIO_AF_DUMMY GPIO_AF14
#define GPIO_AF_EVENT GPIO_AF15

static inline void
configure_pin(portpin_t p) { 
   gpio_set_af(p.port,p.afnum,p.pin);
   gpio_mode_setup(p.port,p.mode,p.pullup,p.pin); 
}

static inline void
write_pin(portpin_t p, bool value) {
   if (value)
     gpio_set(p.port,p.pin);
   else
     gpio_clear(p.port,p.pin);
}

static inline void
write_pins(portpin_t p, uint16_t value) {
   if (value)
     gpio_set(p.port,value & p.pin);
   else
     gpio_clear(p.port,~value & p.pin);
}


/********************* Clocks *************************************************/

#define GPIO_CLOCKS \
           RCC_AHB1ENR_IOPAEN |\
           RCC_AHB1ENR_IOPBEN |\
           RCC_AHB1ENR_IOPCEN |\
           RCC_AHB1ENR_IOPDEN |\
           RCC_AHB1ENR_IOPEEN

#define APB1_CLOCKS \
           RCC_APB1ENR_TIM4EN     |\
           RCC_APB1ENR_TIM3EN     |\
           RCC_APB1ENR_TIM2EN     |\
           RCC_APB1ENR_I2C1EN     |\
           RCC_APB1ENR_SPI3EN

#define APB2_CLOCKS \
           RCC_APB2ENR_SYSCFGEN |\
           RCC_APB2ENR_SPI1EN

#define AHB2_CLOCKS \
           RCC_AHB2ENR_RNGEN


static inline void setup_clocks () {
   //  ahb = 168Mhz, apb1 = 42 MHz apb2 = 84 MHz
   rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

   // enable GPIO CLOCKS
   rcc_peripheral_enable_clock(&RCC_AHB1ENR, GPIO_CLOCKS);

   // APB1 peripherals clocks : tim2 to 4, I2C1, I2S3(SPI)
   rcc_peripheral_enable_clock(&RCC_APB1ENR, APB1_CLOCKS);

   // APB2 peripherals clocks : syscfg (for exti irq lines) + spi1
   rcc_peripheral_enable_clock(&RCC_APB2ENR, APB2_CLOCKS);

   // AHB2 : random number generator
   rcc_peripheral_enable_clock(&RCC_AHB2ENR, AHB2_CLOCKS);
}


/********************* On board leds ******************************************/

// On board leds   (Green LED PD12 ignored :  already used as RTS3)
// 

// Standard output pin configuration 
#define GREEN_0W_LED   ((portpin_t) {GPIOD, GPIO12, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_AF_DUMMY})

// Green led pin driven by TIM4CH1 output
#define GREEN_0W_LED_TIM1CH2   ((portpin_t) {GPIOD, GPIO12, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_AF2})

#define ORANGE_1N_LED  ((portpin_t) {GPIOD, GPIO13, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_AF_DUMMY})
#define RED_2E_LED     ((portpin_t) {GPIOD, GPIO14, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_AF_DUMMY})
#define BLUE_3S_LED    ((portpin_t) {GPIOD, GPIO15, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_AF_DUMMY})

#define F4LEDS         ((portpin_t) {GPIOD, GPIO12|GPIO13|GPIO14|GPIO15, \
                                                    GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_AF_DUMMY})

#define MASK_F4LEDS    0xF
#define SHIFT_F4LEDS   12

static inline void
configure_f4_leds () {
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MASK_F4LEDS << SHIFT_F4LEDS);
}

static inline void
write_f4_leds (uint8_t value) {
  gpio_set   (GPIOD, ( value & MASK_F4LEDS) << SHIFT_F4LEDS);
  gpio_clear (GPIOD, (~value & MASK_F4LEDS) << SHIFT_F4LEDS);
}

/***************************** USER BP *******************************************/

#define BP_USER_BIT 0
#define BP_USER_MASK 1

#define BP_USER ((portpin_t) {GPIOA, GPIO0, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_AF_EVENT})

static inline void configure_BP_USER() {
	configure_pin(BP_USER);
}


static inline int16_t read_BP_USER() {
	int16_t result = gpio_port_read(GPIOA);
	result = result >> BP_USER_BIT;
	result = result & BP_USER_MASK;
	return result;
}

/***************************** I2C + I2S for audio cs4322 **************************/

#define AUDIO_RESET ((portpin_t) {GPIOD, GPIO4, GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_AF_DUMMY})

#define AUDIO_I2C ((portpin_t) {GPIOB, GPIO6| GPIO9, GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_AF4})

#define AUDIO_I2S_IOC ((portpin_t) {GPIOC, GPIO7 | GPIO10 | GPIO12, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_AF6})
#define AUDIO_I2S_IOA ((portpin_t) {GPIOA, GPIO4, GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_AF6})


static inline void configure_I2C_AUDIO_PINS () {
       configure_pin(AUDIO_RESET);
       configure_pin(AUDIO_I2C);
       gpio_set_output_options (AUDIO_I2C.port, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, AUDIO_I2C.pin);
       write_pin(AUDIO_RESET,false);          // keep in reset state
}

static inline void configure_I2S_AUDIO_PINS () {
       configure_pin(AUDIO_I2S_IOC);
       gpio_set_output_options (AUDIO_I2C.port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AUDIO_I2S_IOC.pin);
       configure_pin(AUDIO_I2S_IOA);
}
#endif
