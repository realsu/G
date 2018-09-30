#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2011/6/14
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

////////////////////////////////////////////////////////////////////////////////// 	
#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记

//驱动器参数
//define data address (A1)
#define MotorPowerAddress								0x00
#define	MotorPowerOn 										0x01
#define	MotorPowerOff										0x00
#define	ModeChoiceAddress								0x02
#define ModeChoiceVelocityPC						0xC4
#define ModeChoicePositionPC						0xD0
#define ModeChoicePositionPluse					0xC0
#define ModeChoiceTorquePC							0xC1

#define VelModSetAccAddress							0x0A
#define VelModSetValAddress							0x06
#define PosModSetHighAddress						0x50
#define PosModSetLowAddress							0x05
#define PosModChangeAbsOrRelaControlAdd	0x51
#define PosModSetVelLimitAddress				0x1D
#define TorModSetValAddress							0x08
#define FineZPointAddress								0x53
#define ClearErrorAddress								0x4A


#define UART1   1
#define UART2   2
#define UART1_2	3


//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void uart2_init(u32 bound);
//int fputc(int ch, FILE *f);
//void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void Uart2ASendStr (u8 *pucStr, u8 ulNum);
void Uart1ASendStr (u8 *pucStr, u8 ulNum);
//以下为驱动器通讯协议

void COM_GIIMBot_DRV_Motor_Mode(unsigned char UARTx, unsigned char MotorOnOrOff);
void COM_GIIMBot_DRV_Mode_Choice(unsigned char UARTx, unsigned char ModeChoice);
//void COM_GIIMBot_DRV_Vel_Mode_Set Acc(unsigned char USRTx, );
void COM_GIIMBot_DRV_Position_Mode(unsigned char UARTx, short Temp_PWM, int Temp_Position);

//short RoundEx(const double dInput);

#endif


