#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

////////////////////////////////////////////////////////////////////////////////// 	
#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���

//����������
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


//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void uart2_init(u32 bound);
//int fputc(int ch, FILE *f);
//void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void Uart2ASendStr (u8 *pucStr, u8 ulNum);
void Uart1ASendStr (u8 *pucStr, u8 ulNum);
//����Ϊ������ͨѶЭ��

void COM_GIIMBot_DRV_Motor_Mode(unsigned char UARTx, unsigned char MotorOnOrOff);
void COM_GIIMBot_DRV_Mode_Choice(unsigned char UARTx, unsigned char ModeChoice);
//void COM_GIIMBot_DRV_Vel_Mode_Set Acc(unsigned char USRTx, );
void COM_GIIMBot_DRV_Position_Mode(unsigned char UARTx, short Temp_PWM, int Temp_Position);

//short RoundEx(const double dInput);

#endif


