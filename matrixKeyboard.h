/*
 * matrixKeyboard.h
 *
 *  Created on: 06/03/2019
 *      Author: Charles
 */

#ifndef MATRIXKEYBOARD_H_
#define MATRIXKEYBOARD_H_

#include "MK64F12.h"
/*binary defines for every keyboard key
 * note cero is not zero, this defines
 * are useful the the function read_keyboard
 * is used, her result can be compared
 * by any of these*/
#define key1 0b11101110
#define key2 0b11101101
#define key3 0b11101011
#define keyA 0b11100111

#define key4 0b11011110
#define key5 0b11011101
#define key6 0b11011011
#define keyB 0b11010111

#define key7 0b10111110
#define key8 0b10111101
#define key9 0b10111011
#define keyC 0b10110111

#define keyAst 0b01111110
#define key0 0b01111101
#define keyHash 0b01111011
#define keyD 0b01110111






/*
 *  brief this function calls init keyboard pins
 *  param[in]=void
 *  param[out ]=void
 * */
void init_keyboard();

/*read keyboar by a sweep of four columns
 * param[out]= 4 bits corresponding to cols and 4 bits to rows, this return can be compared against
 *  keyX defines to find a key pressed. Also if her return is zero means that no key was pressed */
uint8_t read_keyboard();

/* brief : Set the four cols pin as output and the four rows pins as output*/
 void init_keyboard_pins();

 /*Brief: set cols making &0F mask to param Value, used by read_keyboard function*/
 void set_columns_value(uint8_t value);

 /*Brief: return 4 bit value of the four rows of the keyboard*/
 uint8_t read_rows();

 /* BRief : simply makes and or between read of  rows and cols  value set for the keyboard sweep
  * used by read_keyboard function*/
 uint8_t calculate_key(uint8_t cols, uint8_t rows);

 /* returns keyboard last key pressed*/
 uint8_t get_lastKey();
 /*clears globlal variable key pressed*/
 void clear_lastKey();




#endif /* MATRIXKEYBOARD_H_ */
