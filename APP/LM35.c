#define F_CPU 8000000UL
#include<util/delay.h>
#include"../LIB/STD_TYPES.h"
#include"../LIB/BIT_MATH.h"
#include"../MCAL/04-ADC/ADC_intrface.h"


#include"../HAL/01-LCD/LCD_intrface.h"


void main()
{
	


	uint32 Local_uint16MilliVolt;
	uint16 Local_uint8Digital;
	uint16 Local_uint8Temp;
	
	HLCD_VoidInit();
	MADC_VoidInit();
	
	while(1)
	{
		Local_uint8Digital = MADC_uint16StartConversionSync(ADC0);

		Local_uint16MilliVolt = (((uint32)Local_uint8Digital * 5000UL) / 1024UL);
		Local_uint8Temp = (Local_uint16MilliVolt / 10);
		HLCD_VoidSendNumber(Local_uint8Temp);


		_delay_ms(1000);
		HLCD_VoidClearLcd();


	}


}
