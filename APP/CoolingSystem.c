/**************************************************************************************************************************************
* LOG:                                                                   							   								  *
* VERSION					AUTHOR           					DATE 						DESCRIPTION 		   					  *
* 1.0.0 					Kareem Hassaan						25 JAN,2022					- Initial Creation						  *
**************************************************************************************************************************************/
/**************************************************************************************************************************************
* ! Title      	: Time Triggered Tempreture Controlled Fan                                                        					  *
* ! File Name	: main.c                                                       							                      		  *
* ! Description : A Time Triggered system measure the tempreture of an atmosphere of a machine and Controlled the speed of a Fan      *
* ! Compiler  	: GNU AVR cross Compiler                                            							                      *
* ! Target 	  	: Atmega32 Micro-Controller                                         							                      *
* ! Layer 	  	: APP	                  						                                         							  *
**************************************************************************************************************************************/

/**************************************************** Library Inclusions *************************************************************/
#define F_CPU 8000000UL
#include<util/delay.h>
#include"../LIB/STD_TYPES.h"
#include"../LIB/BIT_MATH.h"
/************************************************** Lower Layer Inclusions ***********************************************************/
#include"../SERVICES/01-Shceduler/SCH_intrface.h"

#include"../HAL/01-LCD/LCD_intrface.h"

#include"../MCAL/01-DIO/DIO_intrface.h"
#include"../MCAL/03-GIE/GIE_intrface.h"
#include"../MCAL/04-ADC/ADC_intrface.h"
#include"../MCAL/05-Timers/TIMERS_intrface.h"
/********************************************************* Macros ********************************************************************/
#define FAN_PORT						PORTD
#define FAN_PIN							PIN7

#define PRIORITY0						0
#define PRIORITY1						1
#define PRIORITY2						2

#define LM35_TASK_PERIODICITY			4
#define FAN__TASK_PERIODICITY			4
#define LCD__TASK_PERIODICITY			32

#define TEMP_20							20
#define TEMP_25							25
#define TEMP_30							30
#define TEMP_40							40

#define CTC_0							0
#define CTC_100							100
#define CTC_150							150
#define CTC_200							200
#define CTC_255							255

/**************************************************** Global Variables ***************************************************************/
uint16 Global_uint8Temp;

/*************************************************** Functions Prototypes ************************************************************/
void LM35SensorRead(void);
void TempretureOnLCD(void);
void CoolingFan(void);

/*************************************************************************************************************************************/
/****************************************************** Main Function ****************************************************************/
/*************************************************************************************************************************************/
void main()
{
	/*Initialize The LCD*/
	HLCD_VoidInit();
	/*Initialize The ADC Module*/
	MADC_VoidInit();
	/*Initialize The Timer Module*/
	MTIMERS_VoidTimer2Init();
	/*Enable the Global Interrupt*/
	MGIE_voidGlobalInterruptEnable();

	/*Initialize The Fan Pin (PWM Output) to Be Output*/
	MDIO_voidSetPinDirection(FAN_PORT, FAN_PIN,OUTPUT);

	/*Create the System Tasks and Assign to them the Priotity and Periodicity*/
	SSCH_VoidCreateTask(PRIORITY0, FAN__TASK_PERIODICITY, CoolingFan);
	SSCH_VoidCreateTask(PRIORITY1, LM35_TASK_PERIODICITY,LM35SensorRead);
	SSCH_VoidCreateTask(PRIORITY2, LCD__TASK_PERIODICITY,TempretureOnLCD);

	/*Initialization of The Shceduler and start it*/
	SSCH_VoidStart();
	SSCH_VoidInit();

	while (1)
	{

	}
}

/*************************************************************************************************************************************/
/**************************************************** LM35 Sensor Read Function ******************************************************/
/*************************************************************************************************************************************/
void LM35SensorRead(void)
{
	/*Variable To Store The Analog Value in MilliVolt*/
	uint32 Local_uint16MilliVolt;
	/*Variable To Store The Digital Value from The ADC*/
	uint16 Local_uint8Digital;

	/*Reading the result from the ADC Channel*/
	Local_uint8Digital = MADC_uint16StartConversionSync(ADC0);

	/*Calculate the Analog value in MilliVolt*/
	Local_uint16MilliVolt = (((uint32)Local_uint8Digital * 5000UL) / 1024UL);
	/*Calculate the Analog value of the Tempreture in Celsius*/
	Global_uint8Temp = (Local_uint16MilliVolt / 10);
}

/*************************************************************************************************************************************/
/************************************************ Display Temperture On LCD Function *************************************************/
/*************************************************************************************************************************************/
void TempretureOnLCD(void)
{
	/*Clearing the LCD from the old Reading Value*/
	HLCD_VoidClearLcd();

	/*Printing a string "Machine Temp is : " */
	HLCD_VoidSendString("Machine Temp is : ");

	/*Go To specific Place and Printing the value of temp*/
	HLCD_VoidGoTo(ROW1,COL1);
	HLCD_VoidSendNumber(Global_uint8Temp);
	/*Printing the Celsius symbol*/
	HLCD_VoidGoTo(ROW1,COL4);
	HLCD_VoidSendData('C');
}

/*************************************************************************************************************************************/
/******************************************************* Fan Control Function ********************************************************/
/*************************************************************************************************************************************/
void CoolingFan(void)
{

	if(Global_uint8Temp > TEMP_20 && Global_uint8Temp <= TEMP_25)
	{
		/*
			Setting the CM Value to be 100, 
			Turning On the Fan in Normal mode and Normal Speed
		*/
			MTIMERS_voidTimer2SetCompareMatchValue(CTC_100);
			HLCD_VoidGoTo(ROW1,COL7);
			HLCD_VoidSendString("Normal");
	}
	else if(Global_uint8Temp > TEMP_25 && Global_uint8Temp <= TEMP_30)
	{
		/*
			Setting the CM Value to be 150, 
			Turning On the Fan in Medium mode and Medium Speed
		*/
			MTIMERS_voidTimer2SetCompareMatchValue(CTC_150);
			HLCD_VoidGoTo(ROW1,COL7);
			HLCD_VoidSendString("Medium");
	}
	else if(Global_uint8Temp > TEMP_30 && Global_uint8Temp <= TEMP_40)
	{
		/*
			Setting the CM Value to be 200, 
			Turning On the Fan in High Speed mode
		*/
			MTIMERS_voidTimer2SetCompareMatchValue(CTC_200);
			HLCD_VoidGoTo(ROW1,COL7);
			HLCD_VoidSendString("Hot");
	}
	else if(Global_uint8Temp > TEMP_40)
	{
		/*
			Setting the CM Value to be 100, 
			Turning On the Fan in Highist Speed mode
		*/
			MTIMERS_voidTimer2SetCompareMatchValue(CTC_255);
			HLCD_VoidGoTo(ROW1,COL7);
			HLCD_VoidSendString("V.Hot");
	}
	else
	{
		/*
			Turn OFF The Fan The Temp Lower Than 20 c
		*/
			MTIMERS_voidTimer2SetCompareMatchValue(CTC_0);
			HLCD_VoidGoTo(ROW1,COL7);
			HLCD_VoidSendString("Normal");
	}
}