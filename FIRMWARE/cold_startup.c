#include "cold_startup.h"
#include "shield_discoveryf4_wiring.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/cortex.h>

void coldstart() {
   setup_clocks ();


   configure_f4_leds ();

	 configure_BP_USER();
	 configure_SPI1_MEMS();
	 configure_CSSPI1();
	 configure_PE();
	 configure_audio_IN();
	 configure_audio_CLK();
}

