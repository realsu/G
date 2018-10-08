#include "stm32f10x.h"
#include "mp3_usart.h"
#include "LD3320.h"
#include "delay.h"
#include "sys.h"

/******************************************************************************/

int main(void)
{
	//USART_init();
	//printf("\r\n 开始运行LD3320测试程序 \r\n");	
	delay_init();	    	 //延时函数初始化	
	NVIC_Configuration();// 设置中断优先级分组
	MP3_init(9600);	 //串口初始化为9600
	MP3_send_cmd(MP3_VOLUME, 0, 8); // Volume 0-30
   delay_ms(50);
	  // Choose a folder with mp3-files
    // You can use some folders with different languages or different voices
    //MP3_set_folder(1);
    //delay_ms(10);
MP3_send_cmd(MP3_PLAY_FOLDER_FILE, 5, 194); //folder 01..99, file 001..255
	 delay_ms(5000);
	LD3320_main();				//LD3320执行函数
	for(;;)
	{
		
	}
}
/*********************************************END OF FILE**********************/
