/*
 * generador.c
 *
 *  Created on: 14/03/2019
 *      Author: Charles
 */



#include "generador.h"
#include "GPIO.h"
#include "switches_k64.h"


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
void gen_idle(){
}
void generador_cuadrada(){}
void generador_senoidal(){}
void generador_triangular(){}
void generador_led(uint8_t l1_state,uint8_t l2_state)
{
	if(1 == l1_state)
	{
		/*set led l1*/
		GPIO_set_pin(GPIO_D, 1);

	}
	else
	{
		/*clear led l1*/
		GPIO_clear_pin(GPIO_D, 1);
	}
	if(1 == l1_state)
	{
		/* set led l2*/
		GPIO_clear_pin(GPIO_D, 3);

	}
	else
	{
		/*clear led l2*/
		GPIO_set_pin(GPIO_D, 1);
	}

}

/* every state of the generator machine it is defined the same way*/
typedef struct
{
	void (* fptr_signal_to_gen)(void);/* to call  specific generator function*/
	uint8_t led1_state;/* this two members are going to be arguments for generador_led  function*/
	uint8_t led2_state;
	void (* fptr_led_gen)(uint8_t L1_state,uint8_t L2_state);/*to call generador_led function*/
	uint8_t next[NUMBER_OF_STATES];/*to index next state dependig on sw3 value*/
}state_t;

/*MEALY FINITE STATE MACHINE initialization as constant*/
const state_t gen_FSM [NUMBER_OF_STATES]={
										{gen_idle,L1_ON,L2_ON,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_cuadrada,L1_ON,L2_OFF,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_senoidal,L1_OFF,L2_ON,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_triangular,L1_OFF,L2_OFF,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}}

										};

void init_generador_seniales()
{
	//first init the sw3 with his interrputs
	init_sw3(PRIORITY_10,PRIORITY_4,INTR_FALLING_EDGE);

	/*init led 1*/
	GPIO_clock_gating(GPIO_D);
	gpio_pin_control_register_t Pin_PCR_1 = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_D, 1, &Pin_PCR_1);
	GPIO_clear_pin(GPIO_D, 1);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, 22);

	/*init led 3*/
	GPIO_clock_gating(GPIO_D);
	gpio_pin_control_register_t Pin_PCR_3 = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_D, 3, &Pin_PCR_3);
	GPIO_clear_pin(GPIO_D, 3);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT,3);


}
void generador_seniales()
{
	static uint8_t current_state=IDLE;		/*the machine begins on idle*/
	static uint8_t sw3_counter=0;     		/* the sw3 haven´t been pressed on this initialization*/
	/* if the sw3 is pressed a corresponding flag interrupt will be set*/
	if (1 == GPIO_get_irq_status(GPIO_A))
	{
		sw3_counter++;						/* every press will increment the state of the machine*/
		GPIO_clear_irq_status(GPIO_A);		/*flag must be cleared, if it not the state machine will change when it shouldn´t*/

	}
	/*very important to check overflow*/
	if( 4 <=sw3_counter){sw3_counter=0;}
	current_state=sw3_counter;				/*here the current state of machine changes*/
	/*generating current state signal*/
	gen_FSM[current_state].fptr_signal_to_gen();
	/*Giving current state value for leds on generador_led*/
	gen_FSM[current_state].fptr_led_gen(gen_FSM[current_state].led1_state,gen_FSM[current_state].led2_state);

}
