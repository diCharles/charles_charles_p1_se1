/*
 * Passwords.h
 *
 *  Created on: 13/03/2019
 *      Author: Charles
 */

#ifndef PASSWORDS_H_
#define PASSWORDS_H_

 #include "MK64F12.h"
/*			VERY IMPORTANT NOTES . MUST READ                              */
/*inportant note: passwords are defined in passwords.c file */



/*inits pins of FREEDOM k64 for keyboard and interrput for keyboard*/
void init_password();

/*
 * DESCRPICION DE LA FUNCION checkPassword
 *  param[in]: password length, longitud de keys del password
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

/* check three passwords in a non blocking operation, return 1 when a correct password if found, else return zero*/
uint8_t check3Passwords( uint8_t input_key);

/* read keyboard ny interrupt and check password*/


/*this functions will return access flags for 3 specific process */
uint8_t get_system_status();
uint8_t get_motor_status();
uint8_t get_sgnal_generator_status();



#endif /* PASSWORDS_H_ */
