/*
 * rgb.h
 *
 *  Created on: 24/02/2019
 *      Author: Rubén Charles
 */

#ifndef RGB_H_
#define RGB_H_
#include "GPIO.h"

typedef enum {OFF, ON,TOOGLE}
			  led_status;

typedef enum {GREEN,
			  BLUE,
			  PURPLE,
			  RED,
			  YELLOW,
			  WHITE} led_color;

void init_rgb();

void rgb_color(led_color color , led_status status);



#endif /* RGB_H_ */
