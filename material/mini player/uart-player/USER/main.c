#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
//ALIENTEK Mini STM32开发板范例代码3
//串口实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 int main(void)
 {	
	u8 t;
	u8 len;	
	u16 times=0; 
	u8 data[10];
	delay_init();	    	 //延时函数初始化	
	NVIC_Configuration();// 设置中断优先级分组
	MP3_init(9600);	 //串口初始化为9600
	//uart2_init(115200);	 //串口初始化为9600
	LED_Init();		  	 //初始化与LED连接的硬件接口 
	KEY_Init();
	  // Set DFPlayer Volume
   MP3_send_cmd(MP3_VOLUME, 0, 8); // Volume 0-30
   delay_ms(50);
	  // Choose a folder with mp3-files
    // You can use some folders with different languages or different voices
    //MP3_set_folder(1);
    //delay_ms(10);
MP3_send_cmd(MP3_PLAY_FOLDER_FILE, 5, 192); //folder 01..99, file 001..255
	 delay_ms(5000);
    // Play single file from folder
    // This command start playing file 032.mp3 from folder 05
    //MP3_send_cmd(MP3_PLAY_FOLDER_FILE, 5, 194); //folder 01..99, file 001..255
    //Make Voice QUEUE
   // MP3_say(MP3_NO_VALUE, 3148, MP3_NO_VALUE);
    //MP3_say(MP3_NO_VALUE, -35, MP3_NO_VALUE);
    //MP3_say(100, 153, 103);
    //MP3_say(MP3_NO_VALUE, 715, MP3_NO_VALUE);

    while (1)
    {
    	// MP3 Voice QUEUE processing
    	//MP3_queue_processing();
			MP3_send_cmd(MP3_VOLUME, 0, 8); // Volume 0-30
			delay_ms(50);
MP3_send_cmd(MP3_PLAY_FOLDER_FILE, 5, 194); //folder 01..99, file 001..255
    	// Here your code
    	// ...
    	delay_ms(5000);
    	// ...
			LED0=!LED0;
    	//
    }
	 //音量设置为8，7E FF 06 06 00 00 08 FE ED EF
//	data[0]=0x7E; 
//	data[1]=0xFF; 
//	data[2]=0x06; 
//	data[3]=0x06; 
//	data[4]=0x00;
//	data[5]=0x00;
//	data[6]=0x08;
//	data[7]=0xFE;	
//	data[6]=0xED;
//	data[7]=0xEF;	 
	 //音量设置为18，7E FF 06 06 00 00 12 FE E3 EF
	 
	 //指定播放曲目02：7E FF 06 12 00 00 02 FE E7 EF
	 //指定播放曲目01：7E FF 06 12 00 00 01 FE E8 EF
	 //播放下一曲：7E FF 06 01 00 00 00 FE FA EF
	 //播放上一曲：7E FF 06 02 00 00 00 FE F9 EF
	 //暂停：7E FF 06 0E 00 00 00 FE ED EF
	 //播放：7E FF 06 0D 00 00 00 FE EE EF
//	while(1)
//	{
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			//printf("\r\n您发送的消息为:\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART1->DR=USART_RX_BUF[t];
//				while((USART1->SR&0X40)==0);//等待发送结束
//			}
//			//printf("\r\n\r\n");//插入换行
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				//printf("\r\nALIENTEK MiniSTM32开发板 串口实验\r\n");
//				//printf("正点原子@ALIENTEK\r\n\r\n\r\n");
//			}
//			if(times%200==0)
//				{
//					//printf("请输入数据,以回车键结束\r\n");
//					//Uart2ASendStr(data,4);
//					MP3SendStr(data,4);
//				}
//			if(times%50==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
//			delay_ms(10);   
//		}
//	}	 
}


