/*
 * MOTOR.h
 *
 *  Created on: 17/03/2019
 *      Author: José Rodrigo
 */

#ifndef MOTOR_H_
#define MOTOR_H_

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


void motor_sequence_1(void);

void motor_sequence_2(void);

void motor_idle(void);

/*************************************************************************************/
/* This function enables the use of PIT 2 for controlling the ON/OFF sequence of the motor driver
 *  It's interrupt driven*/
void PIT2_init(void);

/*************************************************************************************/
/*Enables GPIO to control pins associated with devide driver:
*
*	PTB11 	pin to control the motor. Power interface required.
*	PTC11 	pin to control LED 2
*	PTC10   pin to control LED 1
*/
void motor_pin_and_leds_init(void);


#endif /* MOTOR_H_ */
