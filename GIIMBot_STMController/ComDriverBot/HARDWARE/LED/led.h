#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED PCout(13)	// PA8
//#define LED1 PDout(2)	// PD2	

#define LED_ON 		GPIO_SetBits(GPIOC , GPIO_Pin_13)
#define LED_OFF 	GPIO_ResetBits(GPIOC , GPIO_Pin_13)

void LED_Init(void);//��ʼ��

		 				    
#endif