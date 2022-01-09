#define F_CPU 8000000UL
#include<util/delay.h>
#include"../LIB/STD_TYPES.h"
#include"../LIB/BIT_MATH.h"



#include"../MCAL/05-Timers/TIMERS_intrface.h"
#include"../MCAL/01-DIO/DIO_intrface.h"
#include"../MCAL/03-GIE/GIE_intrface.h"

void Timer0Test (void)
{
	static uint8 Local_u8PinState = 0;
	static uint16 Local_u16OVFCounts = 0;

		Local_u16OVFCounts++;
		if(Local_u16OVFCounts == 7813)
		{
			Local_u8PinState ^= 1;
			MDIO_voidSetPinValue(PORTA, PIN0, Local_u8PinState);
			Local_u16OVFCounts = 0;
			MTIMERS_voidTimer0SetPreloadValue(128);
		}

}

void(*CBK_FUNC_OVF)(void) = &Timer0Test;

void main()
{
	MDIO_voidSetPinDirection(PORTA, PIN0,OUTPUT);
	MTIMERS_voidTimer0OVFSetCallBack(CBK_FUNC_OVF);
	MGIE_voidGlobalInterruptEnable();
	//MTIMERS_voidTimer0SetPreloadValue(128);
	MTIMERS_VoidTimer0Init();


	while(1)
	{



	}


}
