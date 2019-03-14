/*
 * generador.c
 *
 *  Created on: 14/03/2019
 *      Author: Charles
 */



#include "generador.h"
#include "GPIO.h"

#define NUMBER_OF_STATES 4
#define L1_ON  1
#define L1_OFF 0
#define L2_ON  1
#define L2_OFF 0

typedef enum{
		IDLE,
		CUADRADA,
		SENOIDAL,
		TRIANGULAR

}state_name_t;
void gen_idle(){}
void generador_cuadrada(){}
void generador_senoidal(){}
void generador_triangular(){}
void generador_led(uint8_t l1_state,uint8_t l2_state)
{
	static uint8_t one_init=0;
	if( 0==one_init)
	{
		one_init=1;

	}


}

typedef struct
{
	void (* fptr_signal_to_gen)(void);
	uint8_t led1_state;
	uint8_t led2_state;
	void (* fptr_led_gen)(uint8_t L1_state,uint8_t L2_state);
	uint8_t next[NUMBER_OF_STATES];
}state_t;

/*MEALY FINITE STATE MACHINE*/
const state_t gen_FSM [NUMBER_OF_STATES]={
										{gen_idle,L1_ON,L2_ON,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_cuadrada,L1_ON,L2_OFF,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_senoidal,L1_OFF,L2_ON,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_triangular,L1_OFF,L2_OFF,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}}

										};
void generador_seniales()
{
	static uint8_t current_state=IDLE;
	static uint8_t sw3_counter=0;
	// if (sw3_interrput_status==1) {sw3_counter++}
	if( 4 <=sw3_counter){sw3_counter=0;}
	current_state=sw3_counter;
	/*generating current state signal*/
	gen_FSM[current_state].fptr_signal_to_gen();
	/*Giving current state value for leds on generador_led*/
	gen_FSM[current_state].fptr_led_gen(gen_FSM[current_state].led1_state,gen_FSM[current_state].led2_state);

}
