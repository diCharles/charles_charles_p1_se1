/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    teclado_matricial_4x4.c
 * @brief   Application entry point.
 */

#include "MK64F12.h"
#include "matrixKeyboard.h"
#include "rgb.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
#define  passwordLenght 4
const uint8_t password[passwordLenght]= {keyA,keyB,keyB,keyA};
uint8_t checkPassword(uint8_t passwordLength, uint8_t * password);
int main(void) {
  	/* Init board hardware. */
	init_keyboard();
	init_rgb();

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
		while(0 ==checkPassword(passwordLenght,password));
		//check in succesfull!!!!!
		rgb_color(GREEN,TOOGLE);

    }
    return 0 ;
}
uint8_t checkPassword(uint8_t passwordLength, uint8_t * password)//non bloquing check password
{
	uint8_t static strokeCounter=0;
	uint8_t static userCorrectKeyCntr=0;
	uint8_t keyStroke=read_keyboard();
	if(0 == keyStroke)//if no key is being pressed
	{
		return 0;
	}

	if( password[strokeCounter] == keyStroke)
	{
		userCorrectKeyCntr++;
		strokeCounter++;
		if(strokeCounter == passwordLenght)
		{
			strokeCounter=0; //enables for a new try
			userCorrectKeyCntr=0;
			return 1;
		}
		else
		{

			return 0;
		}
	}
	else
	{
		strokeCounter=0; //enables for a new try
		userCorrectKeyCntr=0;
	}
	return 0 ;

}
