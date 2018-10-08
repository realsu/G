#include "stm32f10x.h"
#include "usart.h"
#include "LD3320.h"

/******************************************************************************/
void DelayLED(uint16 i)
{
	unsigned char a,b;
	for(;i>0;i--)
	        for(b=4;b>0;b--)
	            for(a=113;a>0;a--);	
}

int main(void)
{
	USART_init();

	printf("\r\n 开始运行LD3320测试程序 \r\n");	
//	LED_RED_ON();
//		DelayLED(0xfff);
//		LED_GREEN_ON();
//		DelayLED(0xfff);
//		LED_RED_OFF();
//		DelayLED(0xfff);
//		LED_GREEN_OFF();
//		DelayLED(0xfff);
	LD3320_main();				//LD3320执行函数
	for(;;)
	{
		
	}
}
/*********************************************END OF FILE**********************/
