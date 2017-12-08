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


#include "i2s_audio.h"

#include "discoveryf4_wiring.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/exti.h>

// stm32 ref manual 27.4.4  table 123 page 826
// Mck enabled 44KHz :  PLLI2S N=271 R=2   I2S DIV = 6 ODD=0

#define PLLI2S_NVAL 271
#define PLLI2S_RVAL 2
#define PLLI2S_VAL (PLLI2S_RVAL << 28) | (PLLI2S_NVAL << 6)

#define I2SPR_DIVVAL 6
#define I2SPR_VAL (SPI_I2SPR_MCKOE | I2SPR_DIVVAL)  // ODD=0

// I2S master send, I2S format, 16 bit data & channel
#define I2SCONFIG_VAL ( SPI_I2SCFGR_I2SMOD | \
                        (SPI_I2SCFGR_I2SCFG_MASTER_TRANSMIT << 8) | \
                        (SPI_I2SCFGR_I2SSTD_I2S_PHILIPS << 4) | \
                        (SPI_I2SCFGR_DATLEN_16BIT << 1) \
                      )

uint32_t v2;
volatile uint32_t *ptr;
                        
void i2s_init () {

   // PLL I2S configuration
   rcc_osc_off(RCC_PLLI2S);
   RCC_PLLI2SCFGR=PLLI2S_VAL;
   rcc_osc_on(RCC_PLLI2S);
   rcc_wait_for_osc_ready(RCC_PLLI2S);

   spi_reset(SPI3);  

   // I2S config 
   ptr=&SPI3_I2SCFGR;
   SPI3_I2SCFGR = I2SCONFIG_VAL;

   ptr = &SPI3_I2SPR;
   v2= I2SPR_VAL;
   SPI3_I2SPR = I2SPR_VAL;

   SPI3_I2SCFGR |= SPI_I2SCFGR_I2SE;

   SPI3_DR = 0xaa55;
}
   

