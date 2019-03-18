/*
 * matrixKeyboard.c
 *
 *  Created on: 06/03/2019
 *      Author: Charles
 */



#include "matrixKeyboard.h"
#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"
#include "NVIC.h"


/*set of mask for set_cols function*/
#define NO_KEY 0b1111
#define MaskForColumns 0x0F /*used in set_columns_value()*/
#define	MaskForSetCol_1		(0b0001)
#define	MaskForSetCol_2		(0b0010)
#define	MaskForSetCol_3		(0b0100)
#define	MaskForSetCol_4		(0b1000)

/*defines for  row lecture in read rows function*/
#define row_1 0
#define row_2 1
#define row_3 2
#define row_4 3

#define noKEYread 0b0

/*defines for column sweep of keyboard*/
#define col1 0b1110
#define col2 0b1101
#define col3 0b1011
#define col4 0b0111
/*this array help to index col values for read keyboard function*/
uint8_t cols[4]={col1, col2, col3 , col4};
/* defines for every step of mahine keyboard*/
#define state_1 0
#define state_2 1
#define state_3 2
#define state_4 3
/*this global variable can help to debun the future the key pressed by keyboard when its posible*/

 uint8_t g_last_key=0;

/* machine state implemented for read the keyboard*/
typedef struct
{

	uint8_t previousKey;
	void (*fptr_set_cols) (uint8_t value);
	uint8_t (*fptr_read_rows)(void);//
	uint8_t (*fptr_calculate_key)(uint8_t cols, uint8_t rows);

}g_key_machine_states_t;
 g_key_machine_states_t g_keyboard_machine[4]={   {0,set_columns_value,read_rows,calculate_key},
													{0,set_columns_value,read_rows,calculate_key},
													{0,set_columns_value,read_rows,calculate_key},
													{0,set_columns_value,read_rows,calculate_key}

};
void init_keyboard()
{

	init_keyboard_pins();//j1 header , 8 pins left part seeing board with SD conector to person
	/*THERE IS A LED CONECTED TO PTDO PIN */
	/*THIS LED WILL TOOGLE EVERY TIME THE KEYBOARD PRESSED*/
	/*HELPING DEBUG AND GIVE AID TO USER TO CHECK IF HIS INPUT HAS BEEN PROCESSED*/

	/*using interrupt on pin p  coming out from an and GATE wivh inputs are rows of keyboard*/
	uint32_t input_intr_config = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;
	GPIO_clock_gating(GPIO_D);
	GPIO_pin_control_register(GPIO_D, bit_0, &input_intr_config);
	GPIO_data_direction_pin(GPIO_D, GPIO_PIN_INPUT, bit_0);
	/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	/**Enables and sets a particular interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTD_IRQ,PRIORITY_4);
	NVIC_global_enable_interrupts;

}
uint8_t read_keyboard()
{
	uint8_t current_state=state_1;/* the initial state is always state1*/

	/* a sweep of diferent cols value inits and ends when all cols values
	 * have been set or a key is found, rows_value is diferent that 0b1111 */
	while(state_4 >= current_state )
	{
		/*the first state of the machine is state zero, a cols value for the state is set */
		g_keyboard_machine[current_state].fptr_set_cols(cols[current_state]);
		/* read of rows , is one key its pressed his value will be diferent than 0b1111*/
		uint8_t rows_value =g_keyboard_machine[current_state].fptr_read_rows();
		/* if the read of rows is not equal to 1111 then a key has been pressed*/
		if(0b1111 !=rows_value)
		{

			/*here a key is found, read variable will be the or of rows value and cols value used to find the key*/
			uint8_t read=g_keyboard_machine[current_state].fptr_calculate_key(cols[current_state],rows_value);
			g_last_key=read;/*storing the last key pressed , useful for other functions */
			/* if the key was not already pressed the function will return its value*/
			return read;
			if( read!=g_keyboard_machine[current_state].previousKey)
			{
				g_keyboard_machine[current_state].previousKey=read;/* storing the result for the one shot functionality*/
				g_last_key=read;/*storing the last key pressed , useful for other functions */
				return read;
			}
			else
			{ /*, but if it was already pressed, the function will ignore it, here is the one_shot functionality */
				return 0;
			}

		}
		g_keyboard_machine[current_state].previousKey=0;/*no key was pressed*/
		/* moving machine state, to find the key, it is somewhere :)*/
		current_state++;

	}
	/*no key found :(*/
	g_keyboard_machine[state_1].previousKey=0;
	g_keyboard_machine[state_2].previousKey=0;
	g_keyboard_machine[state_3].previousKey=0;
	g_keyboard_machine[state_4].previousKey=0;
	return 0;

}
uint8_t calculate_key(uint8_t cols, uint8_t rows)
{
	if(0b1111 == rows)
	{
		return 0;
	}
	uint8_t cols_or_rows=cols<<4 | rows;
	return cols_or_rows;

}
void  init_keyboard_pins()
{
	/*
	 * KEYBOARD INPUT AND PIN ASIGNMENT
	 *
	 * 								 _____
	 * PTC3  > KEY BOARD COL   -----|      \
	 * PTC2  > KEY BOARD COL   -----| AND   |_____ INTERRUPT PT0A
	 * PTA2  > KEY BOARD COL   -----|  GATE |		VECTOR
	 * PTB23 > KEY BOARD COL   -----|_____ /
	 * PTA1  < KEY BOARD ROW
	 * PTB9  < KEY BOARD ROW
	 * PTC17 < KEY BOARD ROW
	 * PTC16 < KEY BOARD ROW
	 *
	 * */
	/** the first four blocks correspond to the pins for the columns, wich are outputs*/
	gpio_pin_control_register_t pin_control_register_bit_c_3 = GPIO_MUX1;
	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_3, &pin_control_register_bit_c_3);
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_3);
	GPIO_set_pin(GPIO_C, bit_3);

	gpio_pin_control_register_t pin_control_register_bit_c_2 = GPIO_MUX1;
	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_2, &pin_control_register_bit_c_2);
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_2);
	GPIO_set_pin(GPIO_C, bit_2);

	gpio_pin_control_register_t pin_control_register_bit_a_2 = GPIO_MUX1;
	GPIO_clock_gating(GPIO_A);
	GPIO_pin_control_register(GPIO_A, bit_2, &pin_control_register_bit_a_2);
	GPIO_data_direction_pin(GPIO_A,GPIO_OUTPUT, bit_2);
	GPIO_set_pin(GPIO_A, bit_2);

	gpio_pin_control_register_t pin_control_register_bit_b_23 = GPIO_MUX1;
	GPIO_clock_gating(GPIO_B);
	GPIO_pin_control_register(GPIO_B, bit_23, &pin_control_register_bit_b_23);
	GPIO_data_direction_pin(GPIO_B,GPIO_OUTPUT, bit_23);
	GPIO_set_pin(GPIO_B, bit_23);

	/* Next four blocks correspond to the inputs off the keyboard noted as rows*/

	gpio_pin_control_register_t pin_control_register_bit_a_1 = GPIO_MUX1| GPIO_PS | GPIO_PE;
	GPIO_clock_gating(GPIO_A);
	GPIO_pin_control_register(GPIO_A, bit_1, &pin_control_register_bit_a_1);
	GPIO_data_direction_pin(GPIO_A,GPIO_INPUT, bit_1);
	GPIO_set_pin(GPIO_A, bit_1);

	gpio_pin_control_register_t pin_control_register_bit_b_9 = GPIO_MUX1| GPIO_PS | GPIO_PE;
	GPIO_clock_gating(GPIO_B);
	GPIO_pin_control_register(GPIO_B, bit_9, &pin_control_register_bit_b_9);
	GPIO_data_direction_pin(GPIO_B,GPIO_INPUT, bit_9);
	GPIO_set_pin(GPIO_B, bit_9);

	gpio_pin_control_register_t pin_control_register_bit_c_17 = GPIO_MUX1| GPIO_PS | GPIO_PE;
	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_17, &pin_control_register_bit_c_17);
	GPIO_data_direction_pin(GPIO_C,GPIO_INPUT, bit_17);
	GPIO_set_pin(GPIO_C, bit_17);

	gpio_pin_control_register_t pin_control_register_bit_c_16 = GPIO_MUX1| GPIO_PS | GPIO_PE;
	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_16, &pin_control_register_bit_c_16);
	GPIO_data_direction_pin(GPIO_C,GPIO_INPUT, bit_16);
	GPIO_set_pin(GPIO_C, bit_16);


}
void set_columns_value(uint8_t value)
{
	value&=MaskForColumns;//columns have only four pins
	if( FALSE !=(value & MaskForSetCol_1 ))
	{
		GPIO_set_pin(GPIO_C, bit_3);
	}
	else
	{
		GPIO_clear_pin(GPIO_C, bit_3);
	}

	if( FALSE !=(value & MaskForSetCol_2 ))
	{
		GPIO_set_pin(GPIO_C, bit_2);
	}
	else
	{
		GPIO_clear_pin(GPIO_C, bit_2);
	}

	if( FALSE !=(value & MaskForSetCol_3 ))
	{
		GPIO_set_pin(GPIO_A, bit_2);
	}
	else
	{
		GPIO_clear_pin(GPIO_A, bit_2);
	}
	if( FALSE !=(value & MaskForSetCol_4 ))
	{
		GPIO_set_pin(GPIO_B, bit_23);
	}
	else
	{
		GPIO_clear_pin(GPIO_B, bit_23);
	}

}
uint8_t read_rows()
{
	uint8_t allRowsValue=0;
	if(TRUE ==GPIO_read_pin(GPIO_A, bit_1))
	{
		allRowsValue|=1<<row_1;
	}
	if(TRUE ==GPIO_read_pin(GPIO_B, bit_9))
	{
		allRowsValue|=1<<row_2;
	}
	if(TRUE ==GPIO_read_pin(GPIO_C, bit_17))
	{
		allRowsValue|=1<<row_3;
	}
	if(TRUE ==GPIO_read_pin(GPIO_C, bit_16))
	{
		allRowsValue|=1<<row_4;
	}
	return allRowsValue;
}

/* returns keyboard last key pressed*/
uint8_t get_lastKey()
{
	return g_last_key;
}
/*clears globlal variable key pressed*/
void clear_lastKey()
{
	g_last_key=0;
}
