

/**
 * @file    teclado_matricial_4x4.c
 * @brief   Application entry point.
 *
 * This small program utilizes matricial keyboard
 * and reads his values on pit_0 timer interrupt,
 * it also reads passwords and enables flags when
 * the passwords ingresed by the user are correct
 */

#include "MK64F12.h"
#include "matrixKeyboard.h"
#include "rgb.h"
#include "PIT.h"
#include "MOTOR.h"


#define  largoDeClaves   4//sus usernidades son numero de keys

uint8_t claveMaestra[largoDeClaves]= 			{key1,key2,key3,key4};
uint8_t claveControlMotor[largoDeClaves]=  	{key4,key5,key6,key7};
uint8_t claveGeneradorDeSenial[largoDeClaves]= {key7,key8,key9,key0};

#define SYSTEM_CLOCK (21000000U)
#define DELAY_PIT_1 (0.0001F)
#define ENABLED_ 		1
#define DISABLED_		 0

/* function check password can onlyy check 4  diferennt passwords, password 4 is not defined yet*/
#define password_No0 0 /* asigned for claveMaestra*/
#define password_No1 1 /*asigned for controlMotor*/
#define password_No2 2/*asigned for generador de senial*/
#define passwordNo4  3/* user  free asignation*/

/* global status flags for system , by default all characteristics are disabled*/
uint8_t g_system_status=DISABLED_;
uint8_t g_motor_status =DISABLED_;
uint8_t g_SgnalGenerator_status=DISABLED_;

int main(void)
{

	motor_init();

	while(1)
	{
		FSM_motor();

	}
	return 0 ;
}

