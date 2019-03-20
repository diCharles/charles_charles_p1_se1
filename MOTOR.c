/*
 * MOTOR.c
 *
 *  Created on: 17/03/2019
 *      Author: Charles
 */



#include "MOTOR.h"
#include "NVIC.h"
#include "GPIO.h"
#include "MK64F12.h"
#include "switches_k64.h"
#include "bits.h"
#include "GPIO.h"
#include "rgb.h"
#include "PIT.h"


#define NUMBER_OF_STATES 4
#define L1_ON  1
#define L1_OFF 0
#define L2_ON  1
#define L2_OFF 0

#define SYSTEM_CLOCK (21000000U)  /*Using PIT timer*/
#define DELAY (1)                 /*Interruption each 1 second*/



/*Represents each state atributes*/
typedef struct str_state
{
	void(*fptr_sequence)(void);
	struct str_state * next;

}State_t;



/******************* PUBLIC FUNCTIONS *********************************************/
void motor_init(void)
{
	PIT_clock_gating();
	PIT_enable();
	/*Enabling interrupts for PIT 2*/
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_8);
	NVIC_global_enable_interrupts;

	/*Enables interrupt by pressing SW3*/
	init_sw3(PRIORITY_10,PRIORITY_4,INTR_FALLING_EDGE);

	//DELAY value for periodic interruptions of PIT
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY);

	/*INIT PINS for motor , and leds*/
	/*motor control pin*/
	GPIO_clock_gating(GPIO_B);
	gpio_pin_control_register_t Pin_PCR_11 = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_B, 11, &Pin_PCR_11);
	GPIO_clear_pin(GPIO_B, 11);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, 11);
	/*motor led1 pin */
	GPIO_clock_gating(GPIO_C);
	gpio_pin_control_register_t Pin_PCR_c = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_C, 11, &Pin_PCR_c);
	GPIO_set_pin(GPIO_C, 11);
	GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, 11);
	/*motor led2 pin*/
	GPIO_pin_control_register(GPIO_C, 10, &Pin_PCR_c);
	GPIO_set_pin(GPIO_C, 10);
	GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, 10);

}

/***************************** PRIVATE FUNCTIONS *******************************************/

void motor_leds(led_status led_1_state,led_status led_2_state)
{
	uint8_t both_leds_state = led_1_state + led_2_state;

	switch(both_leds_state)
	{
	case 0: //Idling motor
		GPIO_set_pin(GPIO_C, bit_10);    //LED 1 OFF
		GPIO_set_pin(GPIO_C, bit_11);    //LED 2 OFF
	break;

	case 1: //Sequence 2
		GPIO_set_pin(GPIO_C, bit_10);    //LED 1 OFF
		GPIO_clear_pin(GPIO_C, bit_11);  //LED 2 ON
	break;

	case 2: //Sequence 1
		GPIO_clear_pin(GPIO_C, bit_10);  //LED 1 ON
		GPIO_set_pin(GPIO_C, bit_11);    //LED 2 OFF
	break;

	default://Idling motor
		GPIO_set_pin(GPIO_C, bit_10);    //LED 1 OFF
		GPIO_set_pin(GPIO_C, bit_11);    //LED 2 OFF

	}/*End of switch case*/
}/*End of motor_leds()*/

/**********************************Functions to control the motor *************************************/


//static void motor_sequence_1(void);
/* Order of execution. Motor is ON or OFF during the time specified.
 *
 * STATE OF MOTOR		TIME
 *       ON				1 s
 *       OFF			1 s
 *       ON				3 s
 *       OFF			1 S
*/

//static void motor_sequence_2(void);
/* Order of execution. Motor is ON or OFF during the time specified.
 *
 * STATE OF MOTOR		TIME
 *       ON				4 s
 *       OFF			4 s
 */
//static void motor_idle(void);
/* Order of execution. Motor OFF.
 *
 * STATE OF MOTOR		TIME
 *       OFF	      unlimited
*/

void motor_sequence_1(void)
{
	// motor_leds(ON,OFF); //LED 1 ON, LED 2 OFF
	 static uint8_t pit_event_counter = 0;

	 GPIO_clear_pin(GPIO_C, bit_10);    //LED 1 ON
	 GPIO_set_pin(GPIO_C, bit_11);    //LED 2 OFF

	 if(TRUE ==  PIT_get_interrupt_flag_status(PIT_2))
	 {
		 pit_event_counter ++;
		 PIT_clear_interrupt_flag(PIT_2);
	 }

	 switch(pit_event_counter)
	 {
	 	 case 0:
	 		GPIO_set_pin(GPIO_B, bit_11);
	 	 break;

	 	 case 1:
	 		GPIO_clear_pin(GPIO_B, bit_11);
	 	 break;

	 	 case 2:
	 		GPIO_set_pin(GPIO_B, bit_11);
	 	 break;

	 	 case 3:
	 		GPIO_set_pin(GPIO_B, bit_11);
	 	 break;

	 	 case 4:
	 		GPIO_set_pin(GPIO_B, bit_11);
	 	 break;

	 	 case 5:
	 		GPIO_clear_pin(GPIO_B, bit_11);
	 	 break;

	 	 default:
	 		pit_event_counter = 0;


	 }



}

void motor_sequence_2(void)
{

	//motor_leds(OFF,ON); //LED 1 OFF, LED 2 OFF
	static uint8_t pit_event_counter = 0;
	GPIO_set_pin(GPIO_C, bit_10);    //LED 1 OFF
	GPIO_clear_pin(GPIO_C, bit_11);    //LED 2 ON


	if(TRUE ==  PIT_get_interrupt_flag_status(PIT_2))
	{
		pit_event_counter ++;
		PIT_clear_interrupt_flag(PIT_2);
	}


	switch(pit_event_counter)
	{
	case 0:
		GPIO_set_pin(GPIO_B, bit_11);
		break;

	case 1:
		GPIO_set_pin(GPIO_B, bit_11);
		break;

	case 2:
		GPIO_set_pin(GPIO_B, bit_11);
		break;

	case 3:
		GPIO_set_pin(GPIO_B, bit_11);
		break;

	case 4:
		GPIO_clear_pin(GPIO_B, bit_11);
		break;

	case 5:
		GPIO_clear_pin(GPIO_B, bit_11);
		break;

	case 6:
		GPIO_clear_pin(GPIO_B, bit_11);
		break;

	case 7:
		GPIO_clear_pin(GPIO_B, bit_11);
		break;

	default:
		pit_event_counter = 0;


	}
}

void motor_idle(void)
{
	/*Nothing to do Here for the motor*/
	motor_leds(OFF,OFF);
	GPIO_clear_pin(GPIO_B, bit_11);
}


void FSM_motor()
{
	/*Creating a Finite State Machin for controlling the motor using a linked list*/
	static State_t struct_motor_idle; /*IDLE state*/
	static State_t struct_motor_sequence_1;/*Sequence 1 state*/
	static State_t struct_motor_sequence_2;/*Sequence 2 state*/




	struct_motor_idle.fptr_sequence = motor_idle;
	struct_motor_idle.next = &struct_motor_sequence_1;

	struct_motor_sequence_1.fptr_sequence = motor_sequence_1;
	struct_motor_sequence_1.next = &struct_motor_sequence_2;

	struct_motor_sequence_2.fptr_sequence = motor_sequence_2;
	struct_motor_sequence_2.next = &struct_motor_idle;

	static State_t * current_state = &struct_motor_idle; /*Current state at the HEAD of the list*/

	/*The press of the SW2 enables the change of state*/
	if(TRUE == GPIO_get_irq_status(GPIO_A))
	{

		current_state = current_state->next;

		GPIO_clear_irq_status(GPIO_A);
	}

	/*Executing the current function to control the motor*/
	current_state->fptr_sequence();
	//static void(*function_to_execute)(void);
	//function_to_exectute = current_state->fptr_sequence;






}
