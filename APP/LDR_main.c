#define F_CPU 8000000UL
#include<util/delay.h>
#include"../LIB/STD_TYPES.h"
#include"../LIB/BIT_MATH.h"
#include"../MCAL/04-ADC/ADC_intrface.h"
#include"../HAL/01-LCD/LCD_intrface.h"


void main()
{
	sint32 Local_uint8LED;
	HLCD_VoidInit();
	MADC_VoidInit();
	
	while(1)
	{
		Local_uint8Digital = MADC_uint16StartConversionSync(ADC0);

		Local_uint8LED = MAP(0,255,8,0,Local_uint8Digital);
		HLCD_VoidSendNumber(Local_uint8LED);


		_delay_ms(1000);
		HLCD_VoidClearLcd();

	}


}
