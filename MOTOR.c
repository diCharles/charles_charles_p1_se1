/*
 * MOTOR.c
 *
 *  Created on: 17/03/2019
 *      Author: Charles
 */



#include "MOTOR.h"
#include "GPIO.h"
#include "MK64F12.h"
#include "switches_k64.h"
#include "bits.h"
#include "GPIO.h"
#include "rgb.h"
#include "PIT.h"

/*This functions activate the motor and two leds according to the following FSM:
 *
 * Each time the SW2 is pressed, the state is changed. By default State 1 is executed (IDLE)
 *
 *
 *State 1
 *
 *	idle();       ; LED 1 = OFF  ; LED 2 = OFF;
 *State 2
 *
 *  sequence_1(); ; LED 1 = ON   ; LED 2 = OFF ;
 *State 3
 *
 *  sequence_2(); ; LED 1 = OFF  ; LED 2 = ON ;
 *State 4
 *
 *  idle();       ; LED 1 = OFF  ; LED 2 = OFF;
 */

/*Functions to control the motor */

static void sequence_1(void);
/* Order of execution. Motor is ON or OFF during the time specified.
 *
 * STATE OF MOTOR		TIME
 *       ON				1 s
 *       OFF			1 s
 *       ON				3 s
 *       OFF			1 S
*/
static void sequence_2(void);
/* Order of execution. Motor is ON or OFF during the time specified.
 *
 * STATE OF MOTOR		TIME
 *       ON				4 s
 *       OFF			4 s
 */
static void idle(void);
/* Order of execution. Motor OFF.
 *
 * STATE OF MOTOR		TIME
 *       OFF	      unlimited
*/
#define NUMBER_OF_STATES 4
#define L1_ON  1
#define L1_OFF 0
#define L2_ON  1
#define L2_OFF 0

typedef enum
{
	IDLE,
	SEQUENCE_1,
	SEQUENCE_2,
	IDLE_
}state_name_t;

/*Represents each state atributes*/
typedef struct
{
	void(*fptr_sequence)(void);
	uint8_t led1_state;/* this two members are going to be arguments for generador_led  function*/
	uint8_t led2_state;
	void (* fptr_led_gen)(uint8_t L1_state,uint8_t L2_state);/*to call generador_led function*/
	uint8_t next[NUMBER_OF_STATES];/*to index next state dependig on sw3 value*/

}state_t;


static void sequence_1(void)
{

}

static void sequence_2(void)
{

}

static void idle(void)
{
}
