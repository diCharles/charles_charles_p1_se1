/*
 * Passwords.c
 *
 *  Created on: 13/03/2019
 *      Author: Charles
 */
#include "matrixKeyboard.h"
#include "rgb.h"
#include "matrixKeyboard.h"
#include "PIT.h"
#include "Passwords.h"


#define SYSTEM_CLOCK (21000000U)
#define DELAY_PIT_0 (0.0001F)
#define LED_DEBUG_TECLADO 0


#define  LARGO_DE_CLAVES   4//sus unidades son numero de keys
#define SUPPORTED_PASSWRDS 4
#define ENABLED_ 		1
#define DISABLED_		 0
/* function check password can onlyy check 4  diferennt passwords, password 4 is not defined yet*/
#define PSSWRD_0 0 /* asigned for claveMaestra*/
#define PSSWRD_1 1 /*asigned for controlMotor*/
#define PSSWRD_2 2/*asigned for generador de senial*/
#define PSSWRD_3 3/* user  free asignation*/

#define LAST_KEY_OF_PSSWRD 3
#define PASSWORD_CORRECT 1

uint8_t claveMaestra[LARGO_DE_CLAVES]= 			{key1,key2,key3,key4};
uint8_t claveControlMotor[LARGO_DE_CLAVES]=  	{key4,key5,key6,key7};
uint8_t claveGeneradorDeSenial[LARGO_DE_CLAVES]= {key7,key8,key9,key0};



uint8_t checkPassword(uint8_t passwordLength, uint8_t *  password,uint8_t PasswordNumber,uint8_t inputKey)//non bloquing check password
{
	static uint8_t  strokeCounter[SUPPORTED_PASSWRDS]={0,0,0,0};
	static uint8_t keyStroke[SUPPORTED_PASSWRDS]={0,0,0,0};
	static uint8_t index=0;
	/*if inputKey is equal to zero no key has been pressed*/
	if(0 == inputKey)//if no key is being pressed
	{
		return 0;
	}
	/*otherwise a valid key had been presed, its saved into keystroke array indexed in the corresponding password */
	 keyStroke[PasswordNumber]=inputKey;
	 index=strokeCounter[PasswordNumber];
	/*if key pressed is correct*/
	if( password[index] == keyStroke[PasswordNumber])
	{
		//userCorrectKeyCntr[ PasswordNumber]=userCorrectKeyCntr[ PasswordNumber]+1;
		strokeCounter[ PasswordNumber]=strokeCounter[ PasswordNumber]+1;

		/*if the number of correct keys is equal to the length of the password , wich is three
		 * or the number of correct keys is equal to the password indexed on number of correct keys*/
		if( password[LARGO_DE_CLAVES-1] == keyStroke[PasswordNumber] || (LARGO_DE_CLAVES <=strokeCounter[PasswordNumber]) )
		{
			strokeCounter[ PasswordNumber]=0; //enables for a new try
			return 1;
		}
		else
		{

			return 0;
		}
	}
	else
	{
		strokeCounter[ PasswordNumber]=0; //enables for a new try

	}
	return 0 ;

}

uint8_t check3Passwords(uint8_t input_key )
{




	/*the corresponding flags for each password are going to be toogle when
	 * the input keystrokes from user match the corresponding password
	 * given to checkPassword function*/

	if( PASSWORD_CORRECT == checkPassword(LARGO_DE_CLAVES,claveMaestra,PSSWRD_0,input_key))
	{
		//passwordState|=(0b00000001);
		//master_key=0;
		rgb_color(RED,TOOGLE);
	}

	if( PASSWORD_CORRECT == checkPassword(LARGO_DE_CLAVES,claveControlMotor,PSSWRD_1,input_key))
	{
		//*motorEnable=~(*motorEnable)&(0b00000001);
		//passwordState|=0b00000010;
		rgb_color(GREEN,TOOGLE);
	}

	if( PASSWORD_CORRECT == checkPassword(LARGO_DE_CLAVES,claveGeneradorDeSenial,PSSWRD_2,input_key))
	{

		//passwordState|=0b00000100;
		rgb_color(BLUE,TOOGLE);
	}
	//clear_lastKey();
	return 0;
}
void init_keyboard_for_password_check()
{
	/*the  pit0 is enabled, on his interrupt is going to check the keyboard and passwords for the system*/
	//set_PIT_timer_with_interrupt(PIT_0,SYSTEM_CLOCK , DELAY_PIT_0,
	//PIT_CH0_IRQ, PRIORITY_11);

		//PIT0_callback(&check3Passwords);
	/* Init matricial keyboard pins (rosws and cols). */
		init_keyboard();
		/* the rgb led on the board is set to show colors*/
		init_rgb();
}
