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

#define  largoDeClaves   4//sus unidades son numero de keys

 uint8_t claveMaestra[largoDeClaves]= 			{key1,key2,key3,key4};
 uint8_t claveControlMotor[largoDeClaves]=  	{key4,key5,key6,key7};
 uint8_t claveGeneradorDeSenial[largoDeClaves]= {key7,key8,key9,key0};


int main(void) {
  	/* Init board hardware. */
	init_keyboard();
	init_rgb();


    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {

    	read_keyboard();
    	if( 1 == checkPassword(largoDeClaves,claveMaestra))
    	{
    		rgb_color(YELLOW,TOOGLE);
    	}



    }
    return 0 ;
}

