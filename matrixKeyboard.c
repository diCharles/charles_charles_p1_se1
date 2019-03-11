/*
 * matrixKeyboard.c
 *
 *  Created on: 06/03/2019
 *      Author: Charles
 */


/*  keyboard ISR */
#include "matrixKeyboard.h"
#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"



#define NO_KEY 0b1111
#define MaskForColumns 0x0F /*used in set_columns_value()*/
#define	MaskForSetCol_1		(0b0001)
#define	MaskForSetCol_2		(0b0010)
#define	MaskForSetCol_3		(0b0100)
#define	MaskForSetCol_4		(0b1000)

#define row_1 0
#define row_2 1
#define row_3 2
#define row_4 3

#define noKEYread 0b0


#define col1 0b1110
#define col2 0b1101
#define col3 0b1011
#define col4 0b0111

typedef struct
{
	int8_t out;
	void (*fptr_set_cols) (uint8_t value);
	uint8_t (*fptr_read_rows)(void);//
	uint8_t (*fptr_calculate_key)(uint8_t cols, uint8_t rows);

}key_machine_states_t;
const  key_machine_states_t keyboard_machine[4]={   {0,set_columns_value,read_rows,calculate_key},
													{0,set_columns_value,read_rows,calculate_key},
													{0,set_columns_value,read_rows,calculate_key},
													{0,set_columns_value,read_rows,calculate_key}

};
void init_keyboard()
{

	init_keyboard_pins();//j1 header , 8 pins left part seeing board with SD conector to person


}
uint8_t read_keyboard()
{
	keyboard_machine[0].fptr_set_cols(col1);
	uint8_t rows_value =keyboard_machine[0].fptr_read_rows();
	uint8_t read=keyboard_machine[0].fptr_calculate_key(col1,rows_value);
	if(0b1111 !=rows_value)
	{
		return read;
	}
	keyboard_machine[1].fptr_set_cols(col2);
	 rows_value =keyboard_machine[1].fptr_read_rows();
	 read=keyboard_machine[1].fptr_calculate_key(col2,rows_value);
	if(0b1111 !=rows_value)
	{
		return read;
	}
	keyboard_machine[2].fptr_set_cols(col3);
	rows_value =keyboard_machine[2].fptr_read_rows();
	read=keyboard_machine[2].fptr_calculate_key(col3,rows_value);
	if(0b1111 !=rows_value)
	{
		return read;
	}
	keyboard_machine[3].fptr_set_cols(col4);
	rows_value =keyboard_machine[3].fptr_read_rows();
	read=keyboard_machine[3].fptr_calculate_key(col4,rows_value);
	if(0b1111 !=rows_value)
	{
		return read;
	}

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
