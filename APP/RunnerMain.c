/**************************************************** Library Inclusions *************************************************************/
#define F_CPU 8000000UL
#include<util/delay.h>
#include"../LIB/STD_TYPES.h"
#include"../LIB/BIT_MATH.h"
/************************************************** Lower Layer Inclusions ***********************************************************/
#include"../MCAL/DIO/DIO_intrface.h"

/************************************************** Self Layer Inclusions ************************************************************/

/************************************************** Self Files Inclusions ************************************************************/
/*Must be with that order maybe change Private with config only*/
#include"../HAL/LCD/LCD_intrface.h"

/*Drawing animated Tree*/
uint8 TreePattern[]={ 0b00000100,
		  0b00010100,
		  0b00001010,
		  0b00010101,
		  0b00010101,
		  0b00001110,
		  0b00000100,
		  0b00000100};

/*Drawing  Runner*/
uint8 RunnerPattern[]={ 0b00001110,
		  0b00010111,
		  0b00011111,
		  0b00011111,
		  0b00011100,
		  0b00010111,
		  0b00010100,
		  0b00010100};
uint8 ittr;
void main(void)
{
	/*Initialize the LCD*/
	HLCD_VoidInit();
	/*Set pin0 in port c input for button*/
	MDIO_voidSetPinDirection(PORTC, PIN0,INPUT);
	/*Set pin0 in port c Pulled up for button*/
	MDIO_voidSetPinValue(PORTC, PIN0,HIGH);

	while(1)
	{
		for(ittr = 0;ittr<16;ittr++)
		{
			/*Drawing animated Tree*/
			HLCD_VoidSendCustomPattern(TreePattern, PATTERN_POSITION_0, ROW1, 16-ittr);
			HLCD_VoidSendCustomPattern(TreePattern, PATTERN_POSITION_0, ROW1, 9-ittr);

			if(MDIO_uint8GetPinValue(PORTC, PIN0) == 0)
			{
				/*Jumping animated Runner*/
				HLCD_VoidSendCustomPattern(RunnerPattern, PATTERN_POSITION_1, ROW0,COL5);

			}
			else
			{
				/*Drawing animated Runner*/
				HLCD_VoidSendCustomPattern(RunnerPattern, PATTERN_POSITION_1, ROW1,COL5);
			}
			_delay_ms(100);
			/*Clearing the LCD*/
			HLCD_VoidClearLcd();
		}
	}

}
