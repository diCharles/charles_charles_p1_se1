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

void system_control();
/*FLAGS FOR RUN PROCESS*/
uint8_t g_run_motor=FALSE;
uint8_t g_run_generator=FALSE;
/*flag for stop running the process motor and generator after press A or B and insert corresponding password*/
uint8_t g_stop_motor=FALSE;
uint8_t g_stop_generator =FALSE;
/*this flag will be set everytime a password correct is found*/
uint8_t g_a_correct_password = FALSE;
/* the passwords are checked on the pit3 flag each 200ms aproximately*/
static uint8_t g_check_keyboard = FALSE;

flags_t g_activation_by_A_or_B_keys;
int main(void) {

	/* activation flags of motor and generator*/
	g_activation_by_A_or_B_keys.enable_motor=		0;/*this flag will toogle when  keyA is pressed*/
	g_activation_by_A_or_B_keys.enable_generator= 0;/*this flag will toogle when  keyB is pressed*/
	/* the rgb led on  board is set to show colors*/
	init_rgb();

	init_password();/* init  interrupt pind0 conected to and gate, pit 3 for debounce and keyboard*/

	/* initialization of signal generator*/
	init_generador_seniales();
	/*initialization of motor*/
	motor_init();
	/*set all cols in zero enables for a interrupt by any keyboard key*/
	set_columns_value(0);


	while(1) {


		system_control();


		if( TRUE ==g_run_generator)
		{
			generador_seniales();
		}
		if(TRUE == g_run_motor)
		{
			FSM_motor();
		}

	}
	return 0 ;
}
void system_control()
{
	/*flags for password LEDs*/
	static uint8_t turn_on_green_LED=FALSE;
	static uint8_t turn_on_red_LED=FALSE;

	/*if any key is pressed he and gate will be zero, the and interrupt on pin PD0 is launched*/
	if(TRUE == GPIO_get_irq_status(GPIO_D))
	{
		/* read the keyboard whenever its possible*/
		g_check_keyboard=TRUE;
		/*clear interrupt flag*/


	}
	/*the pit is used for debounce of keyboard*/
	if(PIT_get_interrupt_flag_status(PIT_3))
	{
		/*if g_check_keyboard was set by interrupt */
		if(TRUE == g_check_keyboard)
		{   /*the pit has a periodic interrupt each 100ms*/
			/*the keyboard is read at least in 100 ms or in 200ms at max*/
			static uint8_t at_least_2_delays=0;
			at_least_2_delays++;
			if (2 == at_least_2_delays)
			{
				/*two delays for the pit already pass,lets read the keyboard*/
				uint8_t read =read_keyboard();
				/*set all cols in zero enables for a interrupt by any keyboard key*/
				set_columns_value(0);
				/*here the 3 passwords are checked reading the last key pressed by user */
				g_a_correct_password =check3Passwords(get_lastKey() );/*if correct password appears his signal will be stort*/
				/*the key has been processed, its time to clean it*/
				clear_lastKey();

				/*enable for a new keyboard read with debounce with pit*/
				at_least_2_delays=0;
				/* generator process is or disabled whenever  key B is pressed*/
				if(keyB ==read      && (TRUE == get_sgnal_generator_status()) && (TRUE == get_system_status()) )
				{
					/*every time key B is pressed generator activation will be  toggled*/
					if(FALSE == g_run_generator)
					{	/*if the  generator was not running*/
						g_activation_by_A_or_B_keys.enable_generator=TRUE;
					}
					else
					{//when the motor generator was running
						g_activation_by_A_or_B_keys.enable_generator=FALSE;/* flag to enable it is cleared*/

					}
				}
				else if((keyA ==read) && (TRUE == get_motor_status()) && (TRUE == get_system_status()))
				{
					if(FALSE == g_run_motor)
					{
						g_activation_by_A_or_B_keys.enable_motor=TRUE;

					}
					else
					{
						g_activation_by_A_or_B_keys.enable_motor=FALSE;
					}
				}
				/*clearing flag for read the keyboard, it must be set again by interrupt on port D*/
				g_check_keyboard=FALSE;
				GPIO_clear_irq_status(GPIO_D);
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
							g_run_motor=TRUE;
							//FSM_motor();
						}


					}
					else
					{
						if(FALSE == g_activation_by_A_or_B_keys.enable_motor)
						{
							/* stoping the generator*/
							rgb_color(GREEN,OFF);
							g_run_motor=FALSE;

						}
					}


					if(TRUE == get_sgnal_generator_status() )
					{
						/* here signal generator is activated but no enabled*/
						if(TRUE == g_activation_by_A_or_B_keys.enable_generator)
						{
							rgb_color(BLUE,ON);/*led indicating activation of system*/
							g_run_generator=TRUE;
							g_stop_generator=FALSE;
						}


					}
					else
					{
						/* here signal generator is activated but no enabled*/
						if(FALSE == g_activation_by_A_or_B_keys.enable_generator)
						{
							/* stoping the generator*/
							rgb_color(BLUE,OFF);
							g_run_generator=FALSE;

						}
					}


				}
				else
				{
					/*the system is disaled here*/
					rgb_color(RED,OFF);
				}

			}
		}

		PIT_clear_interrupt_flag(PIT_3);
	}
}
