#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
//ALIENTEK Mini STM32�����巶������3
//����ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
 int main(void)
 {	
	u8 t;
	u8 len;	
	u16 times=0; 
	u8 data[10];
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_Configuration();// �����ж����ȼ�����
	MP3_init(9600);	 //���ڳ�ʼ��Ϊ9600
	//uart2_init(115200);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
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
	 //��������Ϊ8��7E FF 06 06 00 00 08 FE ED EF
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
	 //��������Ϊ18��7E FF 06 06 00 00 12 FE E3 EF
	 
	 //ָ��������Ŀ02��7E FF 06 12 00 00 02 FE E7 EF
	 //ָ��������Ŀ01��7E FF 06 12 00 00 01 FE E8 EF
	 //������һ����7E FF 06 01 00 00 00 FE FA EF
	 //������һ����7E FF 06 02 00 00 00 FE F9 EF
	 //��ͣ��7E FF 06 0E 00 00 00 FE ED EF
	 //���ţ�7E FF 06 0D 00 00 00 FE EE EF
//	while(1)
//	{
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			//printf("\r\n�����͵���ϢΪ:\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART1->DR=USART_RX_BUF[t];
//				while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//			}
//			//printf("\r\n\r\n");//���뻻��
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				//printf("\r\nALIENTEK MiniSTM32������ ����ʵ��\r\n");
//				//printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
//			}
//			if(times%200==0)
//				{
//					//printf("����������,�Իس�������\r\n");
//					//Uart2ASendStr(data,4);
//					MP3SendStr(data,4);
//				}
//			if(times%50==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
//			delay_ms(10);   
//		}
//	}	 
}


