/*
 * DAC.h
 *
 *  Created on: 03/03/2019
 *      Author: Rubén Charles
 */

#ifndef DAC_H_
#define DAC_H_

#include "MK64F12.h"

/*Intialize the DAC 0*/
void DAC_init(void);
/*Takes a float value between -1 and 1. Then restricts that value into a range of
 * 0 to 4095.
**/
void digital_float_to_DAC(float convolution_value);
/*Takes a 16 bit integer and sets de value to the DAC ouput*/
void integer_to_DAC(uint16_t dac_value);

#endif /* DAC_H_ */
