/*
 * main.c
 *
 *  Created on: 14/03/2019
 *      Author: Usuario
 */
#include "MK64F12.h"
#include "generador.h"
int main()
{
	init_generador_seniales();
	while(1)
	{
		 generador_seniales();
	}
return 0;
}

