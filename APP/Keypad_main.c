#define F_CPU 8000000UL

#include"../../LIB/STD_TYPES.h"
#include"../../LIB/BIT_MATH.h"

#include"../HAL/LCD/LCD_intrface.h" 
#include"../HAL/KEYPAD/KEYPAD_intrface.h"  

void main()
{
	HLCD_VoidInit();
	HKPD_VoidInit();
	
	uint8 Local_uint8PressedKey;
	while(1)
	{
		Local_uint8PressedKey = HKPD_uint8GetPressedKey();
		
		if( != 0xFF)
		{
			HLCD_VoidSendData(Local_uint8PressedKey);
		}
	}
}