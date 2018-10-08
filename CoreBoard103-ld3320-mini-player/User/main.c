#include "stm32f10x.h"
#include "mp3_usart.h"
#include "LD3320.h"
#include "delay.h"
#include "sys.h"

/******************************************************************************/

int main(void)
{
	//USART_init();
	//printf("\r\n ��ʼ����LD3320���Գ��� \r\n");	
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_Configuration();// �����ж����ȼ�����
	MP3_init(9600);	 //���ڳ�ʼ��Ϊ9600
	MP3_send_cmd(MP3_VOLUME, 0, 8); // Volume 0-30
   delay_ms(50);
	  // Choose a folder with mp3-files
    // You can use some folders with different languages or different voices
    //MP3_set_folder(1);
    //delay_ms(10);
MP3_send_cmd(MP3_PLAY_FOLDER_FILE, 5, 194); //folder 01..99, file 001..255
	 delay_ms(5000);
	LD3320_main();				//LD3320ִ�к���
	for(;;)
	{
		
	}
}
/*********************************************END OF FILE**********************/
