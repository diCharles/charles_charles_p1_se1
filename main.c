#include "MK64F12.h"
#include "matrixKeyboard.h"
#include "rgb.h"
#include "PIT.h"
#include "Passwords.h"
#define  largoDeClaves   4//sus unidades son numero de keys



#define SYSTEM_CLOCK (21000000U)

#define ENABLED_ 		1
#define DISABLED_		 0

typedef struct{
	uint8_t enable_motor:1;
uint8_t enable_generator:1;
}flags_t;

flags_t activation_by_A_or_B_keys;
int main(void) {

	/* activation flags of motor and generator*/
	activation_by_A_or_B_keys.enable_motor=		0;/*this flag will toogle when  keyA is pressed*/
	activation_by_A_or_B_keys.enable_generator= 0;/*this flag will toogle when  keyB is pressed*/
	/* the rgb led on  board is set to show colors*/
	init_rgb();

	init_password();/* init  interrupt pind0 conected to and gate, pit 3 for debounce and keyboard*/
	/* the passwords are checked on the pit3 flag each 200ms aproximately*/
	static uint8_t check_keyboard=FALSE;

	while(1) {
		/*set all cols in zero enables for a interrupt by any keyboard key*/
		set_columns_value(0);
		/*if any key is pressed he and gate will be zero, the and interrupt on pin PD0 is launched*/
		if(TRUE == GPIO_get_irq_status(GPIO_D))
		{
			check_keyboard=TRUE;
		}
		if(PIT_get_interrupt_flag_status(PIT_3))
		{
			if(TRUE == check_keyboard)
			{
				static uint8_t at_least_2_delays=0;
				at_least_2_delays++;
				if (2 == at_least_2_delays)
				{
					uint8_t read =read_keyboard();

					check3Passwords(get_lastKey() );
					clear_lastKey();
					GPIO_clear_irq_status(GPIO_D);
					at_least_2_delays=0;
					if(keyB ==read      && (TRUE == get_sgnal_generator_status()))
					{
						activation_by_A_or_B_keys.enable_generator=~activation_by_A_or_B_keys.enable_generator;
					}
					else if((keyA ==read) && (TRUE == get_motor_status()))
					{
						activation_by_A_or_B_keys.enable_motor=~activation_by_A_or_B_keys.enable_motor;
					}
					check_keyboard=FALSE;
				}
			}

			PIT_clear_interrupt_flag(PIT_3);
		}
		/**simulation of main off practica 1*/
		if(TRUE == get_system_status())
		{
			/* here the system is activated*/
			rgb_color(RED,ON);

			if(TRUE == get_motor_status())
			{
				/*here motor is activated but not enabled*/
				if(TRUE == activation_by_A_or_B_keys.enable_motor)
				{
					/* motor proccess activated*/
					rgb_color(GREEN,ON);
				}
				else
				{
					rgb_color(GREEN,OFF);
				}
			}

			if(TRUE == get_sgnal_generator_status())
			{
				/* here signal generator is activated but no enabled*/
				if(TRUE == activation_by_A_or_B_keys.enable_generator)
				{
					rgb_color(BLUE,ON);
				}
				else
				{
					rgb_color(BLUE,OFF);
				}
			}

		}
		else
		{
			/*the system is disaled here*/
			rgb_color(RED,OFF);
		}


	}
	return 0 ;
}
