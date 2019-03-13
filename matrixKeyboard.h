/*
 * matrixKeyboard.h
 *
 *  Created on: 06/03/2019
 *      Author: Charles
 */

#ifndef MATRIXKEYBOARD_H_
#define MATRIXKEYBOARD_H_

#include "MK64F12.h"
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
#define supportedPasswords 4

static uint8_t g_last_key_seen =0;
/* function check password can onlyy check 4  diferennt passwords, password 4 is not defined yet*/
#define password_No0 0 /* asigned for claveMaestra*/
#define password_No1 1 /*asigned for controlMotor*/
#define password_No2 2/*asigned for generador de senial*/
#define passwordNo4  3/* user  free asignation*/

/* global status flags for system , by default all characteristics are disabled*/


void init_keyboard();
uint8_t read_keyboard();
 void init_keyboard_pins();
 void set_columns_value(uint8_t value);
 uint8_t read_rows();
 uint8_t calculate_key(uint8_t cols, uint8_t rows);

 /*
  * DESCRPICION DE LA FUNCION checkPassword
  *  param[in]: password legth, longitud de keys del password
  *  param[in]: password apuntador al password, tipicamente se pasa el arreglo por valor
  *  param[in]: passwordNumber hay 4 passwords soportados por la función, hay que indicarle a esta cual revisará
  *
  *  param[out]: 1 si el password ingresado es correcto, 0 si aun no se ha terminado de ingresar o si este es erroneo
  *
  *  Nota 1: la funcion checkpassword requiere de llamar primero a la la función read keyboard, antes de cada llamada a esta
  *  		para adquirir un nuevo key a revisar
  *  Nota 2:
  *  */
 uint8_t checkPassword(uint8_t passwordLength, uint8_t *  password,uint8_t PasswordNumber,uint8_t inputKey);//non bloquing check password

 /*
  * Brief: this function checks three passwords with the function checkpassword
  * param[in] : pointers to access status for each password
  *
  * */
 void check3Passwords( uint8_t *loggin1,uint8_t *logging2,uint8_t *loggin3);

#endif /* MATRIXKEYBOARD_H_ */
