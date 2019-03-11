/*
 * matrixKeyboard.h
 *
 *  Created on: 06/03/2019
 *      Author: Charles
 */

#ifndef MATRIXKEYBOARD_H_
#define MATRIXKEYBOARD_H_

#include "MK64F12.h"

void init_keyboard();
uint8_t read_keyboard();
 void init_keyboard_pins();
 void set_columns_value(uint8_t value);
 uint8_t read_rows();
 uint8_t calculate_key(uint8_t cols, uint8_t rows);
#endif /* MATRIXKEYBOARD_H_ */
