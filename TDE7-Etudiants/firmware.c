#include "util.h"

uint8_t pos = 0;
uint8_t sens = 0;


/** Define pour vous aider dans vos tests **/
//#define Q1
//#define Q2
#define Q3
//#define Q4

// Handler du timer qui vous interesse
void tim2_isr (void) {
	timer_clear_flag(TIM2,TIM_SR_CC3IF);
	#if defined(Q2) || defined(Q3) // chenillard
		pos= (pos +1)%3;
		set_leds(1<<pos);

	#endif
}

// Timer "témoin" pour voir si la carte marche bien!
void tim4_isr (void) {
	timer_clear_flag(TIM4,TIM_SR_CC1IF);
}

// À remplir pour set_leds!
#define MASK_3LEDS 0x7
#define SHIFT_3LEDS 13
/**
 * Arguments: une valeur sur 3 bits significatifs
 * Descr: Cette fonction écrit la valeur sur le GPIO des leds afin d'allumer toutes celles dont le bit correspondant est à 1
 * Note1: Les autres leds doivent être éteintes
 * Note2: La led verte (bit numéro 1) étant utilisée par un timer, cette fonction ne peut ni ne doit l'influencer!
 **/
void set_leds(uint8_t val)
{
	gpio_set(GPIOD, (val&MASK_3LEDS)<< SHIFT_3LEDS);
	gpio_clear(GPIOD, ((~val)&MASK_3LEDS)<< SHIFT_3LEDS);

}

/**
 * Arguments: Aucun
 * Sorties: 1 si le bouton a été préssé, 0 sinon
 * Descr: Cette fonction lit le GPIO du bouton poussoir et retourne le bit significatif
 **/
uint16_t read_BP()
{
	return 0; // À changer!
}

void exti0_isr()
{
	exti_reset_request(EXTI0);
  #if 1
	#endif
}

int main (void) {

	coldstart(); // Config de base de la carte
	util_setup(); // Timer setup, déjà fait pour vous!

	// Q1: Allumez une led:
	// D'abord écrivez une fonctions qui vous permet d'écrire sur la zone correspondant aux leds
	// Ensuite, utilisez la pour allumer les différentes leds, observez et passer à la suite après avoir allumé toutes les leds (1 à 1, puis ensemble).
#ifdef Q1
	set_leds(1);
#endif

	// Q2: Chenillard
	// En utilisant le traitant (ou handler) d'interuptions tim2_isr déjà fourni, écrivez un chenillard, e.g. allumez une leds, puis la suivante à chaque interuption du timer
	// Que veut dire la ligne déjà présente dans le traitant?

	// Q3: inverser le sens du chenillard
	// Ajoutez ici même le code pour scruter la valeur du bouton poussoir et ainsi
	// inverser le sens du chenillard lorsque une pression est faite sur ce bouton
	// Compléter la fonction read_BP afin de faire la scrutation

	// Q3_BONUS: Changez read_BP afin de reperer un front montant du bouton (réduit effet rebon)

#ifdef Q3
// À compléter

#endif
	// Q4: Refaite la question du dessus avec une interruption (explication dans le sujet)
  // Pensez bien à supprimer le code au dessus
#ifdef Q4
// À compléter
#endif

	while(1);
	return 0;
}
