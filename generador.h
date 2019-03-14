/*
 * generador.h
 *
 *  Created on: 14/03/2019
 *      Author: Usuario
 */



#ifndef GENERADOR_H_
#define GENERADOR_H_

#include "MK64F12.h"

/*empty state, it does nothing*/
void gen_idle();
/*generates with the DAC a 5 hz squares signal*/
void generador_cuadrada();

/* generates with the DAC a 5 hz sinusoidal signal*/
void generador_senoidal();

/*geneates with the DAC a 5hz triangular signal */
void generador_triangular();

/*generates status leds for generador FSM*/
void generador_led(uint8_t l1_state,uint8_t l2_state);

/* init the SW3 with interrup, PIT1 and pins for external leds for generador LED function*/
void init_generador_seniales();

/*general process of generador de señales FSM*/
void generador_seniales();


#endif /* GENERADOR_H_ */
