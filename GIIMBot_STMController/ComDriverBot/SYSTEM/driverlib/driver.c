#include "driver.h"
void Position_Mode_Test(unsigned char UARTx, short Temp_PWM, int Temp_Position)
{	
	COM_GIIMBot_DRV_Motor_Mode(UARTx,MotorPowerOn);
	COM_GIIMBot_DRV_Mode_Choice(UARTx,ModeChoicePositionPC);
	COM_GIIMBot_DRV_Position_Mode(UARTx,Temp_PWM,Temp_Position);
	delay_ms(500);
}

