/*
 * DAC.c
 *
 *  Created on: 03/03/2019
 *      Author: Rubén Charles
 */

#include "DAC.h"
#include "MK64F12.h"
#define DAC_DATA_LOW 0x00FF
#define DAC_DATA_HIGH 0x0F00

void DAC_init(void)
{
	//Activates DAC0 and DAC 1.
	SIM->SCGC2 = SIM_SCGC2_DAC0_MASK;
	//DAC control register 0 =  DAC enable | Reference select = 2
	DAC0->C0 = CMP_DACCR_DACEN_MASK | CMP_DACCR_VRSEL_MASK;
	//Data to be converted to analog voltage
	DAC0->DAT[0].DATL = 0;//DATL is 8 bits size, represents DATA0
	DAC0->DAT[0].DATH = 0;//DATH has 4 bits available, representsDATA1
}

void digital_float_to_DAC(float convolution_value)
{
	//Converts the convolution  (expected in intervals from -1 to 1) values into DAC's 12 bits
	uint16_t value_to_DAC = (uint16_t)(convolution_value*2047)+2048;

	DAC0->DAT[0].DATL = (uint8_t)(DAC_DATA_LOW & (value_to_DAC)); //To store  the first 8 bits

	DAC0->DAT[0].DATH = (uint8_t)((DAC_DATA_HIGH & value_to_DAC) >> 8);   //To store the other last 8 bits
}

void integer_to_DAC(uint16_t dac_value)
{
	DAC0->DAT[0].DATL = (DAC_DATA_LOW & dac_value); //To store  the first 8 bits
	DAC0->DAT[0].DATH = (DAC_DATA_HIGH & dac_value) >> 8;//To store the second 4 bits
}
