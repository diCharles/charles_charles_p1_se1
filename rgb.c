//#include "GPIO.h"
#include "rgb.h"
#include "MK64F12.h"

//LED rojo init

void rgb_color(led_color color , led_status status);
void led_on_or_off(gpio_port_name_t PORT ,uint8_t pin_number ,led_status status);

void init_rgb()
{
	//Initialization of the 3 leds of the board in OFF.
	//RED
	GPIO_clock_gating(GPIO_B);
	gpio_pin_control_register_t Pin_PCR_22 = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_B, 22, &Pin_PCR_22);
	GPIO_set_pin(GPIO_B, 22);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, 22);

	//BLUE

	GPIO_clock_gating(GPIO_B);
	gpio_pin_control_register_t Pin_PCR_21 = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_B, 21, &Pin_PCR_21);
	GPIO_set_pin(GPIO_B, 21);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, 21);

	//GREEN
	GPIO_clock_gating(GPIO_E);
	gpio_pin_control_register_t Pin_PCR_26 = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_E, 26, &Pin_PCR_26);
	GPIO_set_pin(GPIO_E, 26);
	GPIO_data_direction_pin(GPIO_E, GPIO_OUTPUT, 26);
}

void rgb_color(led_color color , led_status status)
{
	switch(color)
	{

		case GREEN:
			led_on_or_off(GPIO_E ,26 ,status);
		break;

		case BLUE:
			led_on_or_off(GPIO_B ,21 ,status);
		break;

		case PURPLE:
			led_on_or_off(GPIO_B ,22 ,status); //RED color
			led_on_or_off(GPIO_B ,21 ,status); //BLUE color
		break;

		case RED:
			led_on_or_off(GPIO_B ,22 ,status);

		break;

		case YELLOW:
			led_on_or_off(GPIO_B ,22 ,status); //RED color
			led_on_or_off(GPIO_E ,26 ,status); //GREN color
		break;

		case WHITE:
			led_on_or_off(GPIO_B ,22 ,status); //RED color
			led_on_or_off(GPIO_E ,26 ,status); //GREN color
			led_on_or_off(GPIO_B ,21 ,status); //BLUE color

		break;

		default:
			//Turns off all leds
			led_on_or_off(GPIO_B ,22 ,OFF); //RED color
			led_on_or_off(GPIO_E ,26 ,OFF); //GREN color
			led_on_or_off(GPIO_B ,21 ,OFF); //BLUE color
	}
}

void led_on_or_off(gpio_port_name_t PORT ,uint8_t pin_number ,led_status status)
{
	if(ON == status)//led on
	{
		GPIO_clear_pin(PORT, pin_number);
	}
	else if(OFF == status) //led off
	{
		GPIO_set_pin(PORT, pin_number);
	}
	else if (TOOGLE == status )
	{
		GPIO_toogle_pin(PORT,pin_number);
	}

}
