#include "MK64F12.h"
#include "matrixKeyboard.h"
#include "rgb.h"
#include "PIT.h"
#include "Passwords.h"
#include "generador.h"
#include "MOTOR.h"

#define  largoDeClaves   4//sus unidades son numero de keys



#define SYSTEM_CLOCK (21000000U)

#define ENABLED_ 		1
#define DISABLED_		 0

typedef struct{
	uint8_t enable_motor:1;
uint8_t enable_generator:1;
}flags_t;

flags_t g_activation_by_A_or_B_keys;
int main(void) {

	/* activation flags of motor and generator*/
	g_activation_by_A_or_B_keys.enable_motor=		0;/*this flag will toogle when  keyA is pressed*/
	g_activation_by_A_or_B_keys.enable_generator= 0;/*this flag will toogle when  keyB is pressed*/
	/* the rgb led on  board is set to show colors*/
	init_rgb();

	init_password();/* init  interrupt pind0 conected to and gate, pit 3 for debounce and keyboard*/
	/* the passwords are checked on the pit3 flag each 200ms aproximately*/
	uint8_t a_correct_password = FALSE;
	static uint8_t check_keyboard = FALSE;
	/* initialization of signal generator*/
	init_generador_seniales();
	/*initialization of motor*/
	motor_init();


	while(1) {
		/*set all cols in zero enables for a interrupt by any keyboard key*/
		set_columns_value(0);
		/*if any key is pressed he and gate will be zero, the and interrupt on pin PD0 is launched*/
		if(TRUE == GPIO_get_irq_status(GPIO_D))
		{
			/* read the keyboard whenever its possible*/
			check_keyboard=TRUE;
			/*clear interrupt flag*/


		}
		/*the pit is used for debounce of keyboard*/
		if(PIT_get_interrupt_flag_status(PIT_3))
		{
			/*if check_keyboard was set by interrupt */
			if(TRUE == check_keyboard)
			{   /*the pit has a periodic interrupt each 100ms*/
				/*the keyboard is read at least in 100 ms or in 200ms at max*/
				static uint8_t at_least_2_delays=0;
				at_least_2_delays++;
				if (2 == at_least_2_delays)
				{
					/*two delays for the pit already pass,lets read the keyboard*/
					uint8_t read =read_keyboard();
					/*here the 3 passwords are checked reading the last key pressed by user */
					a_correct_password =check3Passwords(get_lastKey() );/*if correct password appears his signal will be stort*/
					/*the key has been processed, its time to clean it*/
					clear_lastKey();
					/*enable for a new keyboard read with debounce with pit*/
					at_least_2_delays=0;
					/* generator process is or disabled whenever  key B is pressed*/
					if(keyB ==read      && (TRUE == get_sgnal_generator_status()) && (TRUE == get_system_status()))
					{
						/*every time key B is pressed generator activation will be  toggled*/
						g_activation_by_A_or_B_keys.enable_generator=~g_activation_by_A_or_B_keys.enable_generator;
					}
					else if((keyA ==read) && (TRUE == get_motor_status()) && (TRUE == get_system_status()))
					{
						/*every time key A is pressed  motor activation will be  toggled*/
						g_activation_by_A_or_B_keys.enable_motor=~g_activation_by_A_or_B_keys.enable_motor;
					}
					/*clearing flag for read the keyboard, it must be set again by interrupt on port D*/
					check_keyboard=FALSE;
					GPIO_clear_irq_status(GPIO_D);
				}
			}

			PIT_clear_interrupt_flag(PIT_3);
		}

		/*the system will be enabled the the password 1234 was ingresed*/
		if(TRUE == get_system_status())
		{
			/* here the system is activated*/
			rgb_color(RED,ON);/*led indicating activation of system*/

			if(TRUE == get_motor_status())
			{

				/*here motor is activated but not enabled*/
				if(TRUE == g_activation_by_A_or_B_keys.enable_motor)
				{
					/* motor proccess activated*/
					rgb_color(GREEN,ON);/*led indicating activation of system*/
					FSM_motor();
				}
				else
				{
					rgb_color(GREEN,OFF);
				}


			}


			if(TRUE == get_sgnal_generator_status())
			{
				/* here signal generator is activated but no enabled*/
				if(TRUE == g_activation_by_A_or_B_keys.enable_generator)
				{
					rgb_color(BLUE,ON);/*led indicating activation of system*/
					generador_seniales();
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
