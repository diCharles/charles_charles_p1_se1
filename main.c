#include "MK64F12.h"
#include "matrixKeyboard.h"
#include "rgb.h"
#include "PIT.h"
#include "Passwords.h"
#define  largoDeClaves   4//sus unidades son numero de keys



#define SYSTEM_CLOCK (21000000U)
#define DELAY_PIT_1 (0.001F)
#define ENABLED_ 		1
#define DISABLED_		 0



int main(void) {
	/* Init matricial keyboard pins (rosws and cols). */
	init_keyboard();
	/* the rgb led on the board is set to show colors*/
	init_rgb();
	/*the  pit0 is enabled, on his interrupt is going to check the keyboard and passwords for the system*/
	set_PIT_timer_with_interrupt(PIT_3,SYSTEM_CLOCK , DELAY_PIT_1,
	PIT_CH3_IRQ, PRIORITY_11);
	//uint8_t pit_inter_status = FALSE;
	init_keyboard_for_password_check();
	/* the passwords are checked on the pit0 ISR each 9 ms approximately*/

	while(1) {
		set_columns_value(0);
		if(TRUE == GPIO_get_irq_status(GPIO_D))
		{
			uint8_t keyboard_lecture=read_keyboard();
			check3Passwords(get_lastKey() );
			set_columns_value(0);

			if(keyboard_lecture != 0)
			{
				rgb_color(RED,TOOGLE);
			}
			GPIO_clear_irq_status(GPIO_D);
		}
		uint8_t key= get_lastKey();
		clear_lastKey();
	}
	return 0 ;
}
