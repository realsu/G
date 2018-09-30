/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ��3.5.0�汾�⽨�Ĺ���ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "bsp_spi.h"
#include "bsp_led.h" 
#include "delay.h" 
#include "WS2812B.h" 
#include "WS2811.h"
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	  int i;
	  /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	  delay_ms(500);

	  //WS_Init();
	  LED_SPI_LowLevel_Init();
  	while(1)
		{  
			  WS_SetAll();
        LED_SPI_Update(WsDat,41);
//			ColorToColor(0x000000zh,0x0f4400);
//			ColorToColor(0x0f4400,0x000000);
//		  ColorToColor(0x000000,0x440044);
//			ColorToColor(0x440044,0x000000);
//			ColorToColor(0x000000,0x004444);
//			ColorToColor(0x004444,0x000000);
//      ColorToColor(0x000000,0x444400);
//			ColorToColor(0x444400,0x000000);
//			ColorToColor(0x000000,0x440000);
//			ColorToColor(0x440000,0x000000);
//			ColorToColor(0x000000,0x004400);
//			ColorToColor(0x004400,0x000000);
//			ColorToColor(0x000000,0x000044);
//			ColorToColor(0x000044,0x000000);
//			for(i=0;i<96;i++)
//			{
//				WS_SetAll();
				delay_ms(100);
//			}
//			
			
		}
	  
}

/*********************************************END OF FILE**********************/

