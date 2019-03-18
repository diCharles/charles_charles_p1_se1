#include "MK64F12.h"
#include "matrixKeyboard.h"
#include "rgb.h"
#include "PIT.h"
#include "Passwords.h"
#define  largoDeClaves   4//sus unidades son numero de keys



#define SYSTEM_CLOCK (21000000U)
#define DELAY_PIT_3 (0.1F)
#define ENABLED_ 		1
#define DISABLED_		 0



int main(void) {
	/* Init matricial keyboard pins (rosws and cols). */
	init_keyboard();
	/* the rgb led on  board is set to show colors*/
	init_rgb();
	/*the  pit3 is enabled, on his interrupt is going to check the keyboard and passwords for the system*/
	set_PIT_timer_with_interrupt(PIT_3,SYSTEM_CLOCK , DELAY_PIT_3,
	PIT_CH3_IRQ, PRIORITY_9);
	//uint8_t pit_inter_status = FALSE;
	init_keyboard_for_password_check();
	/* the passwords are checked on the pit0 ISR each 9 ms approximately*/
	uint8_t check_keyboard=FALSE;
	while(1) {
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
				read_keyboard();
				check_keyboard=FALSE;
				check3Passwords(get_lastKey() );
				clear_lastKey();
				GPIO_clear_irq_status(GPIO_D);
				at_least_2_delays=0;
				}
			}

			PIT_clear_interrupt_flag(PIT_3);
		}


	}
	return 0 ;
}
