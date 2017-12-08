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
#ifndef __UTIL_H__
#define __UTIL_H__
#include "cold_startup.h"
#include "discoveryf4_wiring.h"

// lib cmsis : nvic irq and systoick management

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

// libopencm3 : peripherals

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/syscfg.h>    // for exti lines configuration


uint32_t read_rng(uint8_t is_from_interrupt);

void rng_enable(uint8_t with_interrupt);

void plus_plus_handler(void);

void tim4_setup(uint32_t prescaler,uint32_t period); 

// Prescaler : divide 42MHz apb1 freq to timer input
void tim2_setup(uint32_t prescaler,uint32_t period); 

void exti0_setup (void);

void do_test_exti0 (void);

void util_setup(void);
#endif
