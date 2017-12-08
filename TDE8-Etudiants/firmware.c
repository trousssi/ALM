#include "util.h"
#include <libopencm3/stm32/f4/rng.h>

uint32_t rnd = 0;
uint32_t waited_time = 0;

// Timer pour le sleep
void tim2_isr (void)
{
	waited_time++;
	// Laissez a la fin!
	timer_clear_flag(TIM2,TIM_SR_CC3IF);
  __asm__ volatile ("nop");
  __asm__ volatile ("nop");

}

/**
 * Enables the RNG
 * If with_interrupt is set, then RNG will be configured so as to generate interruts
 * every time a new random number is generated
 **/
void rng_enable(uint8_t with_interrupt)
{

		RNG_CR |= RNG_CR_RNGEN;
		if (with_interrupt)
		{
			nvic_enable_irq(NVIC_HASH_RNG_IRQ);
			RNG_CR |= RNG_CR_IE;
		}
}

/**
 * Read a random number from generator
 * Prerequisite: RNG must be enabled correctly!
 * Returns a random 32 bit integer
 * Should check for errors depending on mode(interrupt/passive)!
 **/
uint32_t read_rng(uint8_t is_from_interrupt)
{
	if(is_from_interrupt)
	{
		while ((RNG_SR & RNG_SR_CEIS) || (RNG_SR & RNG_SR_SEIS)) {}
			return RNG_DR;
	}
	else
	{
		while (!(RNG_SR & RNG_SR_DRDY)) {}
		if ((RNG_SR & RNG_SR_CECS) || (RNG_SR & RNG_SR_SECS))
		{
			return -1;
		}
		return RNG_DR;
	}
}

/**
 * RNG interrupt handler
 **/
void hash_rng_isr(void)

{
	rnd = read_rng(1);
}

void sleep(uint32_t s)
{
	waited_time = 0;
	while(waited_time < 10*s) {}
}

int main (void) {

	coldstart(); // Config de base de la carte
	util_setup(); // Timer setup, deja fait pour vous!
	uint32_t randoum;
	rng_enable(1);
	// Juste pour vérifier que la carte tourne! A supprimer dans vos éxos!


	while(1) {
		sleep((rnd%5)+1);
		write_f4_leds(1<<(rnd%4));
	}
	return 0;
}
