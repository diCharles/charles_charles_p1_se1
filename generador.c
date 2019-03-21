/*
 * generador.c
 *
 *  Created on: 14/03/2019
 *      Author: Charles
 */



#include "generador.h"
#include "GPIO.h"
#include "switches_k64.h"


#define NUMBER_OF_STATES 4
#define L1_ON  1
#define L1_OFF 0
#define L2_ON  1
#define L2_OFF 0
#define SINUSOIDAL_LUT_VALUES 161

typedef enum{
		IDLE,
		CUADRADA,
		SENOIDAL,
		TRIANGULAR

}state_name_t;
/* Values for SINE wave generation*/
static const uint16_t lut_for_sine[SINUSOIDAL_LUT_VALUES] =
{
	2000, 2079, 2157, 2235, 2313, 2390,	2467, 2543, 2618, 2692,	2765, 2837,	2908, 2977,	3045, 3111,
	3176, 3238,	3299, 3358,	3414, 3469,	3521, 3571,	3618, 3663,	3705, 3745,	3782, 3816,	3848, 3876,
	3902, 3925,	3945, 3962,	3975, 3986,	3994, 3998,	4000, 3998,	3994, 3986,	3975, 3962,	3945, 3925,
	3902, 3876,	3848, 3816,	3782, 3745,	3705, 3663,	3618, 3571,	3521, 3469,	3414, 3358,	3299, 3238,
	3176, 3111,	3045, 2977,	2908, 2837,	2765, 2692,	2618, 2543,	2467, 2390,	2313, 2235,	2157, 2079,
	2000, 1921,	1843, 1765,	1687, 1610,	1533, 1457,	1382, 1308,	1235, 1163,	1092, 1023,	 955,  889,
	 824,  762,	 701,  642,	 586,  531,	 479,  429,	 382,  337,	 295,  255,  218,  184,	 152,  124,
	  98,  	75,	  55, 	38,   25,	14,	   6,	 2,	   0,    2,	   6,	14,   25,   38,	  55, 	75,
	  98,  124,  152,  184,  218,  255,  295,  337,	 382,  429,  479,  531,  586,  642,  701,  762,
	 824,  889,  955, 1023, 1092, 1163, 1235, 1308,	1382, 1457, 1533, 1610,	1687, 1765, 1843, 1921,
	2000
};

void gen_idle()
{
	//does nothing
}
void generador_cuadrada(){
	static uint16_t square_ctr=0;
	square_ctr++;
	if(2000 > square_ctr )
	{
	DAC0->DAT[0].DATL = 0xFF;
	DAC0->DAT[0].DATH = 0xFF;
	}
	else
	{
		DAC0->DAT[0].DATL = 0x00;
		DAC0->DAT[0].DATH = 0x00;
	}
	if(4000 < square_ctr)
	{
		square_ctr=0;
	}
}
void generador_senoidal()
{
	static uint16_t sample = 0; //To navigate across the LUT of the sine
		static int8_t dac_init_flag=0;
		/*init dac if it havent been initialized*/
		if( 0 ==dac_init_flag)
		{
			SIM->SCGC2 = 0x1000;
			DAC0->C0 = 0xC0;
			DAC0->DAT[0].DATL = 0;
			DAC0->DAT[0].DATH = 0;


			dac_init_flag=1;
		}
		/* checking LUT indexing overflow*/
		if(SINUSOIDAL_LUT_VALUES-1 <=sample )
		{
			sample =0;
		}
		/*increment LUT indexer*/
		sample++;
		/*set an output to the DAC*/
		DAC0->DAT[0].DATL = 0x00FF & lut_for_sine[sample];
		DAC0->DAT[0].DATH = lut_for_sine[sample]>>8;
}
void generador_triangular()
{
	static int16_t triangular=0;
	if(4000 > triangular)
	{
		DAC0->DAT[0].DATL = 0x00FF & triangular;
		DAC0->DAT[0].DATH = triangular>>8;
		triangular++;
	}
	else
	{
		triangular=0;
	}
}
void generador_led(uint8_t l1_state,uint8_t l2_state)
{
	if(1 == l1_state)
	{
		/*set led l1*/
		GPIO_set_pin(GPIO_D, 1);

	}
	else
	{
		/*clear led l1*/
		GPIO_clear_pin(GPIO_D, 1);
	}
	if(1 == l1_state)
	{
		/* set led l2*/
		GPIO_clear_pin(GPIO_D, 3);

	}
	else
	{
		/*clear led l2*/
		GPIO_set_pin(GPIO_D, 1);
	}

}

/* every state of the generator machine it is defined the same way*/
typedef struct
{
	void (* fptr_signal_to_gen)(void);/* to call  specific generator function*/
	uint8_t led1_state;/* this two members are going to be arguments for generador_led  function*/
	uint8_t led2_state;
	void (* fptr_led_gen)(uint8_t L1_state,uint8_t L2_state);/*to call generador_led function*/
	uint8_t next[NUMBER_OF_STATES];/*to index next state dependig on sw3 value*/
}state_t;

/*MEALY FINITE STATE MACHINE initialization as constant*/
const state_t gen_FSM [NUMBER_OF_STATES]={
										{gen_idle,L1_ON,L2_ON,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_cuadrada,L1_ON,L2_OFF,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_senoidal,L1_OFF,L2_ON,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}},
										{generador_triangular,L1_OFF,L2_OFF,generador_led,{IDLE,CUADRADA,SENOIDAL,TRIANGULAR}}

										};

void init_generador_seniales()
{
	//first init the sw3 with his interrputs
	init_sw3(PRIORITY_10,PRIORITY_4,INTR_FALLING_EDGE);

	/*init led 1*/
	GPIO_clock_gating(GPIO_D);
	gpio_pin_control_register_t Pin_PCR_1 = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_D, 1, &Pin_PCR_1);
	GPIO_clear_pin(GPIO_D, 1);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, 22);

	/*init led 3*/
	GPIO_clock_gating(GPIO_D);
	gpio_pin_control_register_t Pin_PCR_3 = GPIO_MUX1;
	GPIO_pin_control_register(GPIO_D, 3, &Pin_PCR_3);
	GPIO_clear_pin(GPIO_D, 3);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT,3);

	SIM->SCGC2 = 0x1000;
	DAC0->C0 = 0xC0;
	DAC0->DAT[0].DATL = 0;
	DAC0->DAT[0].DATH = 0;

}
void generador_seniales()
{
	static uint8_t current_state=IDLE;		/*the machine begins on idle*/
	static uint8_t sw3_counter=0;     		/* the sw3 haven´t been pressed on this initialization*/
	/* if the sw3 is pressed a corresponding flag interrupt will be set*/
	if (1 == GPIO_get_irq_status(GPIO_A))
	{
		sw3_counter++;						/* every press will increment the state of the machine*/
		GPIO_clear_irq_status(GPIO_A);		/*flag must be cleared, if it not the state machine will change when it shouldn´t*/

	}
	/*very important to check overflow*/
	if( 4 <=sw3_counter){sw3_counter=0;}
	current_state=sw3_counter;				/*here the current state of machine changes*/
	/*generating current state signal*/
	gen_FSM[current_state].fptr_signal_to_gen();
	/*Giving current state value for leds on generador_led*/
	gen_FSM[current_state].fptr_led_gen(gen_FSM[current_state].led1_state,gen_FSM[current_state].led2_state);

}
