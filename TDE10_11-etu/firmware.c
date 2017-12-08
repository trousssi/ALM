#include "systick.h"
#include "cold_startup.h"
#include "i2c_audio.h"

#include "discoveryf4_wiring.h"

// lib cmsis : nvic irq and systoick management

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

// libopencm3 : peripherals

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/syscfg.h>    // for exti lines configuration
#include <libopencm3/stm32/spi.h>


uint16_t syscount = 0;
uint8_t val=0;
uint32_t sysms = 0;

void sys_tick_handler () {
	syscount++;
	if (syscount == 500) {
		syscount = 0;
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


#define NOTEFREQUENCY 0.015		//frequency of saw wave: f0 = 0.5 * NOTEFREQUENCY * 48000 (=sample rate)
#define NOTEAMPLITUDE 500.0		//amplitude of the saw wave

typedef struct {
	float tabs[8];
	float params[8];
	uint8_t currIndex;
} fir_8;

volatile uint32_t sampleCounter = 0;
volatile int16_t sample = 0;
float sawWave = 0.0;
float filteredSaw = 0.0;

// a very crude FIR lowpass filter
float updateFilter(fir_8* filt, float val)
{
	uint16_t valIndex;
	uint16_t paramIndex;
	float outval = 0.0;

	valIndex = filt->currIndex;
	filt->tabs[valIndex] = val;

	for (paramIndex=0; paramIndex<8; paramIndex++)
	{
		outval += (filt->params[paramIndex]) * (filt->tabs[(valIndex+paramIndex)&0x07]);
	}

	valIndex++;
	valIndex &= 0x07;

	filt->currIndex = valIndex;

	return outval;
}

void initFilter(fir_8* theFilter)
{
	uint8_t i;

	theFilter->currIndex = 0;

	for (i=0; i<8; i++)
		theFilter->tabs[i] = 0.0;

	theFilter->params[0] = 0.01;
	theFilter->params[1] = 0.05;
	theFilter->params[2] = 0.12;
	theFilter->params[3] = 0.32;
	theFilter->params[4] = 0.32;
	theFilter->params[5] = 0.12;
	theFilter->params[6] = 0.05;
	theFilter->params[7] = 0.01;
}

uint8_t tab[256];
uint8_t lu;

int main (void) {
	coldstart();
	fir_8 filt;
	write_f4_leds(0);
	systick_setup();

	configure_i2c_audio();
	//lu = read_cs(0x1);

	// *** POUR LE TDE11 : ***

	// Initialisation du filtre (voir ci-dessus)
	// a completer...
	initFilter(&filt);
	while(1)
	  {
	    if ((SPI_SR(SPI3) & SPI_SR_TXE))
	      {
		// envoi de la donnee 16 bits "sample" via le bus I2S (interface SPI3)
		// a completer...
		spi_send(SPI3, sample);
		//only update on every second sample to insure that L & R ch. have the same sample value
		if (sampleCounter & 0x00000001)
		  {
		    sawWave += NOTEFREQUENCY;
		    if (sawWave > 1.0)
		      sawWave -= 2.0;

		    filteredSaw = updateFilter(&filt, sawWave);

		    sample = (int16_t)(NOTEAMPLITUDE*filteredSaw);
		  }
		sampleCounter++;
	      }

	    if (sampleCounter==48000)
	      {
		// completer pour allumage d'une led...
				write_f4_leds(1<<0);
	      }
	    else if (sampleCounter == 96000)
	      {
		sampleCounter = 0;
		// completer pour allumage d'une autre led...
			write_f4_leds(1<<3);
	      }
	  }
}
