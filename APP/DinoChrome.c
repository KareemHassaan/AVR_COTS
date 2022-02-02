/**************************************************************************************************************************************
* LOG:                                                                   							   								  *
* VERSION					AUTHOR           					DATE 						DESCRIPTION 		   					  *
* 1.0.0 					Kareem Hassaan						21 DEC,2021					- Initial Creation						  *
**************************************************************************************************************************************/
/**************************************************************************************************************************************
* ! Title      	: Dino Chrome Game                                                     							                      *
* ! File Name	: main.c     		                                                  							                      *
* ! Description : This file has the implementation of Dino Chrome Game   	        							                      *
* ! Compiler  	: GNU AVR cross Compiler                                            							                      *
* ! Target 	  	: Atmega32 Micro-Controller                                         							                      *
* ! Layer 	  	: APP                   						                                         							  *
**************************************************************************************************************************************/
/**************************************************** Library Inclusions *************************************************************/
#define F_CPU 8000000UL
#include<util/delay.h>
#include"../LIB/STD_TYPES.h"
#include"../LIB/BIT_MATH.h"
/************************************************** Lower Layer Inclusions ***********************************************************/
#include"../MCAL/01-DIO/DIO_intrface.h"
#include"../MCAL/02-EXT_INTERRUPT/EXTI_intrface.h"
#include"../MCAL/03-GIE/GIE_intrface.h"

#include"../HAL/01-LCD/LCD_intrface.h"
/********************************************************** Macros *******************************************************************/
#define GAME_OVER 1
/***************************************************** Global Variables **************************************************************/
uint8  ittr = 0;
uint8  CBKFunc_uint8LoopCounter = 0;
uint16 Global_uint16Mode = 0;
uint16 Global_uint16Score = 0;

/*Drawing animated Cactus Array*/
uint8 TreePattern[] = { 0b00000100,
					    0b00010100,
					    0b00001010,
					    0b00010101,
					    0b00010101,
					    0b00001110,
					    0b00000100,
					    0b00000100};

/*Drawing  animated Dino Array*/
uint8 RunnerPattern[] = { 0b00001110,
					      0b00010111,
					      0b00011111,
					      0b00011111,
					      0b00011100,
					      0b00010111,
					      0b00010100,
					      0b00010100};

/*************************************************** Functions Prototype *************************************************************/
void CBKFunc_VoidJump();
void Start_Screen();
/*************************************************************************************************************************************/
/******************************************************* Main Function ***************************************************************/
/*************************************************************************************************************************************/
void main(void)
{
	/*Initialize the LCD*/
	HLCD_VoidInit();
	
	/*Enable The Global Interrupt*/
	MGIE_voidGlobalInterruptEnable();
	/*Initializ The EXTI*/
	M_EXTI_VoidInit();
	/*Setting EXTI Call Back Function*/
	M_EXTI_voidSetCallback(INT0, CBKFunc_VoidJump);
	
	/*Set pin0 in port c input for button*/
	MDIO_voidSetPinDirection(PORTD, PIN2,INPUT);
	/*Set pin0 in port c Pulled up for button*/
	MDIO_voidSetPinValue(PORTD, PIN2,HIGH);

	/*Starting The Game*/
	Start_Screen();

	while(Global_uint16Mode != GAME_OVER)
	{
		for(ittr = 0;ittr<16;ittr++)
		{
			/*Drawing animated Cactus*/
			HLCD_VoidSendCustomPattern(TreePattern, PATTERN_POSITION_0, ROW1, 16-ittr);
			HLCD_VoidSendCustomPattern(TreePattern, PATTERN_POSITION_0, ROW1, 9-ittr);



				/*Drawing animated Dino*/
				HLCD_VoidSendCustomPattern(RunnerPattern, PATTERN_POSITION_1, ROW1,COL5);

				/*Checking if The Dino Crashed With the cactus*/
				if(ittr == 5 || ittr == 12)
				{
					Global_uint16Mode = GAME_OVER;
					ittr = 16;
				}

			_delay_ms(100);

			/*Clearing the LCD*/
			HLCD_VoidClearLcd();
		}
	}

	/*Printing the Score When The Game is over*/
	HLCD_VoidSendString("Game Over");
	HLCD_VoidGoTo(ROW1, COL0);
	HLCD_VoidSendString("Score : ");
	HLCD_VoidSendNumber((Global_uint16Score));

}

/*************************************************************************************************************************************/
/***************************************************** EXTI CBK Function *************************************************************/
/*************************************************************************************************************************************/
void CBKFunc_VoidJump()
{
	for(CBKFunc_uint8LoopCounter = ittr; CBKFunc_uint8LoopCounter < ittr+4; CBKFunc_uint8LoopCounter++)
	{
		/*Counting The User Score*/
		if(CBKFunc_uint8LoopCounter == 5 || CBKFunc_uint8LoopCounter == 12)
		{
			Global_uint16Score++;
		}

		/*Jumping the Dino Over The Cactus*/
		HLCD_VoidSendCustomPattern(RunnerPattern, PATTERN_POSITION_1, ROW0,COL5);
	
		HLCD_VoidSendCustomPattern(TreePattern, PATTERN_POSITION_0, ROW1, 16-CBKFunc_uint8LoopCounter);
		HLCD_VoidSendCustomPattern(TreePattern, PATTERN_POSITION_0, ROW1, 9-CBKFunc_uint8LoopCounter);
	}

	ittr = CBKFunc_uint8LoopCounter;
}

/*************************************************************************************************************************************/
/****************************************************** Start Function ***************************************************************/
/*************************************************************************************************************************************/
void Start_Screen()
{
	/*Count Down 3*/
	HLCD_VoidGoTo(ROW0, COL7);
	HLCD_VoidSendNumber(3);
	_delay_ms(1000);
	HLCD_VoidClearLcd();

	/*Count Down 2*/
	HLCD_VoidGoTo(ROW0, COL7);
	HLCD_VoidSendNumber(2);
	_delay_ms(1000);
	HLCD_VoidClearLcd();

	/*Count Down 1*/
	HLCD_VoidGoTo(ROW0, COL7);
	HLCD_VoidSendNumber(1);
	_delay_ms(1000);
	HLCD_VoidClearLcd();

	/*Printing Start on The Screen*/
	HLCD_VoidGoTo(ROW0, COL5);
	HLCD_VoidSendString("START");
	_delay_ms(1000);
	HLCD_VoidClearLcd();
}
