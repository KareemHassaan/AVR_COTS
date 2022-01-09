#define F_CPU 8000000UL
#include<util/delay.h>
#include"../LIB/STD_TYPES.h"
#include"../LIB/BIT_MATH.h"



#include"../MCAL/05-Timers/TIMERS_intrface.h"

#include"../MCAL/01-DIO/DIO_intrface.h"
#include"../MCAL/04-ADC/ADC_intrface.h"
#include"../HAL/01-LCD/LCD_intrface.h"
#include"../MCAL/03-GIE/GIE_intrface.h"
#include"../MCAL/06-USART/USART_intrface.h"

/*void Timer0Test (void)
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
*/

volatile uint16 Global_uint16PeriodTicks =0;
volatile uint16 Global_uint16OnTicks =0;

void ICU_HW(void)
{
	volatile static uint8 Local_uint8EdgeCounter = 0;
	Local_uint8EdgeCounter++;
	volatile static uint16 Local_uint16Reading1 = 0;
	volatile static uint16 Local_uint16Reading2 = 0;
	volatile static uint16 Local_uint16Reading3 = 0;

	if(Local_uint8EdgeCounter == 1)
	{
		Local_uint16Reading1	=	MTIMERS_uint16Timer1GetICR1Value();


	}
	else if(Local_uint8EdgeCounter == 2)
	{
		Local_uint16Reading2	=	MTIMERS_uint16Timer1GetICR1Value();
		Global_uint16PeriodTicks = (Local_uint16Reading2 - Local_uint16Reading1 )- 1;

		 MTIMERS_voidTimer1ICUSetSenseSignal(FALLING_EDGE);
	}
	else if(Local_uint8EdgeCounter == 3)
	{
		Local_uint16Reading3	=	MTIMERS_uint16Timer1GetICR1Value();

		Global_uint16OnTicks =( Local_uint16Reading3 -  Local_uint16Reading2  )- 1;
		MTIMERS_voidTimer1GICUInterruptControl(DISABLE);
	}

}



void main()
{
	MDIO_voidSetPinDirection(PORTB, PIN3, OUTPUT);
	MDIO_voidSetPinDirection(PORTD, PIN6, INPUT);
	MDIO_voidSetPinDirection(PORTD, PIN1, OUTPUT);
	MDIO_voidSetPinDirection(PORTD, PIN0, INPUT);


	MTIMERS_voidTimer0SetCompareMatchValue(250);
	MTIMERS_VoidTimer0Init();


	 MTIMERS_voidSetCallBack(TIMER1,ICU, &ICU_HW);
	 MTIMERS_voidTimer1ICUSetSenseSignal(RISING_EDGE);
	 MTIMERS_voidTimer1GICUInterruptControl(ENABLE);
	 MGIE_voidGlobalInterruptEnable();


	 MTIMERS_VoidTimer1Init();

	 MUSART_VoidInit();
//	sint16 LoopCounter = 0;
//	MDIO_voidSetPinDirection(PORTB, PIN4, OUTPUT);

	//MADC_VoidInit();
	//HLCD_VoidInit();
//
//	MDIO_voidSetPinDirection(PORTD, PIN4, OUTPUT);
//	MDIO_voidSetPinDirection(PORTD, PIN5, OUTPUT);
//	MDIO_voidSetPinDirection(PORTA, PIN0, INPUT);
//	MTIMERS_voidTimer1SetTopValue(19999);
//	MTIMERS_VoidTimer0Init();



	while(1)
	{

		while(!(Global_uint16PeriodTicks == 0 || Global_uint16OnTicks ==0))
		{

			//MUSART_VoidSendNumber(1212);
			MUSART_VoidSendNumber(Global_uint16OnTicks);
			MUSART_VoidTransimt(' ');
			MUSART_VoidSendNumber(Global_uint16PeriodTicks);
			MUSART_VoidTransimt(' ');
			_delay_ms(2000);

		}








//		HLCD_VoidSendNumber(MAP(0, 1024, 480, 2050, MADC_uint16StartConversionSync(ADC0)));
//		_delay_ms(1000);
//		HLCD_VoidClearLcd();
//		MTIMERS_voidTimer1SetCompareMatchValue(MAP(0, 1024, 480, 2050, MADC_uint16StartConversionSync(ADC0)));

//		MTIMERS_voidTimer1SetChannelBCompareMatchValue(480); // it should be the 0 degree 480
//		_delay_ms(1000);
//		MTIMERS_voidTimer1SetChannelBCompareMatchValue(1150); // it should be 90 degree 1150
//		_delay_ms(1000);
//		MTIMERS_voidTimer1SetChannelBCompareMatchValue(2050); // it should be 180 degree 2050
//		_delay_ms(1000);





/*
		for(LoopCounter = 0; LoopCounter <= 255; LoopCounter++)
		{
			MTIMERS_voidTimer0SetCompareMatchValue(LoopCounter);
			_delay_ms(10);
		}
		for(LoopCounter = 255; LoopCounter >= 0; LoopCounter--)
		{
			MTIMERS_voidTimer0SetCompareMatchValue(LoopCounter);
			_delay_ms(10);
		}*/

	}
}


