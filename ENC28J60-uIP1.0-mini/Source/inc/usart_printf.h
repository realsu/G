/****************************************************************************
* Copyright (C), 2009-2010, www.armfly.com
*
* 文件名: usart_printf.h
* 内容简述: 头文件
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2009-12-27 armfly  创建该文件
*
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_PRINTF_H
#define __USART_PRINTF_H

#include "stdio.h"	
#include "sys.h" 

////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

////////////////////////////////////////////////////////////////////////////////// 	
#define USART2_REC_LEN  		200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记

//驱动器参数
//define data address (A1)
#define MotorPowerAddress							0x00
#define	MotorPowerOn 								0x01
#define	MotorPowerOff								0x00
#define	ModeChoiceAddress							0x02
#define ModeChoiceVelocityPC						0xC4
#define ModeChoicePositionPC						0xD0
#define ModeChoicePositionPluse					    0xC0
#define ModeChoiceTorquePC							0xC1

#define VelModSetAccAddress							0x0A
#define VelModSetValAddress							0x06
#define PosModSetHighAddress						0x50
#define PosModSetLowAddress							0x05
#define PosModChangeAbsOrRelaControlAdd	            0x51
#define PosModSetVelLimitAddress				    0x1D
#define TorModSetValAddress							0x08
#define FineZPointAddress							0x53
#define ClearErrorAddress							0x4A


#define UART1   1
#define UART2   2
#define UART1_2	3

void PrintfLogo(char *strName, char *strDate);
void USART2_Configuration(void);

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void Uart2ASendStr (u8 *pucStr, u8 ulNum);
void Uart1ASendStr (u8 *pucStr, u8 ulNum);
//以下为驱动器通讯协议

void COM_GIIMBot_DRV_Motor_Mode(unsigned char UARTx, unsigned char MotorOnOrOff);
void COM_GIIMBot_DRV_Mode_Choice(unsigned char UARTx, unsigned char ModeChoice);
//void COM_GIIMBot_DRV_Vel_Mode_Set Acc(unsigned char USRTx, );
void COM_GIIMBot_DRV_Position_Mode(unsigned char UARTx, short Temp_PWM, int Temp_Position);
#endif


