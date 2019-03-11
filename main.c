

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

#define  largoDeClaves   4//sus unidades son numero de keys

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

int main(void) {
	/* Init matricial keyboard pins (rosws and cols). */
	init_keyboard();
	/* the rgb led on the board is set to show colors*/
	init_rgb();
	/*the  pit0 is enabled, on his interrupt is going to check the keyboard and passwords for the system*/
	set_PIT_timer_with_interrupt(PIT_1,SYSTEM_CLOCK , DELAY_PIT_1,
			PIT_CH1_IRQ, PRIORITY_11);
	uint8_t pit_inter_status = FALSE;
	/* the passwords are checked on the pit0 ISR each 9 ms approximately*/

	while(1) {

		pit_inter_status= PIT_get_interrupt_flag_status(PIT_1);
		if(TRUE == pit_inter_status)
		{

			PIT_clear_interrupt_flag(PIT_1);
			/*if the passwords are correct the correspondig status flags for the system are enabled, the set
					 * of this flags is done in the pit1 ISR*/
					read_keyboard();

					if( 1 == checkPassword(largoDeClaves,claveMaestra,password_No0))
					{
						rgb_color(RED,TOOGLE);
					}
					read_keyboard();
					if( 1 == checkPassword(largoDeClaves,claveControlMotor,password_No1))
					{
						rgb_color(GREEN,TOOGLE);
					}
					read_keyboard();
					if( 1 == checkPassword(largoDeClaves,claveGeneradorDeSenial,password_No2))
					{
						rgb_color(BLUE,TOOGLE);
					}
					/*reading pit1 interrupt flag set on his ISR*/

		}


	}
	return 0 ;
}

