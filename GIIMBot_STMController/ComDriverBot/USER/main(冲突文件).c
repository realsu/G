#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "ps2.h"
#include "driver.h"

int main(void)
 {	
	//u8 t;
	//u8 len;	
//	u16 times=0; 
//	u8 PS2_KEY = 0; 
//	u8 data[4];
	delay_init();	    	 	//delay function initialization
	NVIC_Configuration();	//set interrupt priority grouping
	uart_init(57600);	 		//uart1 initializationÔºö57600 
	uart2_init(57600);	 	//uart2 initializationÔºö57600
	LED_Init();		  	 		//LED initialization
	PS2_Init();						//PS2 joystick initialization 
  LED_ON;
	delay_ms(50);
	LED_OFF;
	delay_ms(50);
	LED_ON;
	delay_ms(50);
	LED_OFF;
	 LED_ON;
		//set the motor driver mode to power on
		COM_GIIMBot_DRV_Motor_Mode(UART1_2,MotorPowerOn);
		//set the motor driver input mode to Position from PC 
		COM_GIIMBot_DRV_Mode_Choice(UART1,ModeChoicePositionPC);

	 while(1)
	{

		delay_ms(5000);
		LED_OFF;
		
		COM_GIIMBot_DRV_Position_Mode(UART1,40,-10000);
		COM_GIIMBot_DRV_Position_Mode(UART2,40,10000);
		delay_ms(10000);
		LED_ON;
		
		delay_ms(5000);
		LED_OFF;
		
		COM_GIIMBot_DRV_Position_Mode(UART1,40,10000);
		COM_GIIMBot_DRV_Position_Mode(UART2,40,-10000);
		delay_ms(10000);
		LED_ON;
	}


//		PS2_KEY=PS2_DataKey();
//		delay_ms(10);
//		PS2_KEY = 0;
//		PS2_KEY=PS2_DataKey();
//		if(PS2_KEY != 0)
//		{
//		switch(PS2_KEY)
//			{		
//				case PSB_PAD_UP: 		
//					
//				break;  
//				case PSB_PAD_RIGHT:	
//					//printf("\r\n PSB_PAD_RIGHT \r\n"); 	 
//				//…Ë÷√◊™ÀŸ100RPM	
//					data[0]=0x06; 
//					data[1]=0x00; 
//					data[2]=0x1B; 
//					data[3]=0x21; 
//					Uart1ASendStr(data,4);
//					Uart2ASendStr(data,4);				
//				break;
//				case PSB_PAD_DOWN:	
//					//printf("\r\n PSB_PAD_DOWN \r\n");   	
//					//…Ë÷√◊™ÀŸ-100RPM
//					data[0]=0x06; 
//					data[1]=0xFE; 
//					data[2]=0xEF; 
//					data[3]=0xF3;  
//					Uart2ASendStr(data,4);
//					//…Ë÷√◊™ÀŸ100RPM
//					data[0]=0x06; 
//					data[1]=0x01; 
//					data[2]=0x11; 
//					data[3]=0x18;
//					Uart1ASendStr(data,4);
//				break; 
//				case PSB_PAD_LEFT:	
//					//printf("\r\n PSB_PAD_LEFT \r\n");   	
//				//…Ë÷√◊™ÀŸ-100RPM
//					data[0]=0x06; 
//					data[1]=0x00; 
//					data[2]=0x52; 
//					data[3]=0x58; 
//					Uart1ASendStr(data,4);
//					Uart2ASendStr(data,4);
//				break; 
//				case PSB_L2:				
//					//printf("\r\n PSB_L2 \r\n");
//					//Õ£÷πµÁª˙
//					COM_GIIMBot_DRV_Motor_Mode(UART1_2,MotorPowerOff);
//					//COM_GIIMBot_DRV_Motor_Mode(UART1_2,MotorOff);
//				break; 
//				case PSB_L1:				
//					//printf("\r\n PSB_L1 \r\n");	
//					COM_GIIMBot_DRV_Motor_Mode(UART1_2,MotorPowerOn);
//					//COM_GIIMBot_DRV_Motor_Mode(UART1_2,MotorOn);
//				break; 
//				case PSB_R2:      	
//					//printf("\r\n PSB_R2 \r\n");   				
//				break; 
//				case PSB_R1:      	
//					//printf("\r\n PSB_R1 \r\n");   				
//				break;     
//				case PSB_TRIANGLE:	
//					//◊Û¬÷…Ë÷√◊™ÀŸ200RPM
//					data[0]=0x06; 
//					data[1]=0x02; 
//					data[2]=0x22; 
//					data[3]=0x2A; 
//					Uart2ASendStr(data,4);
//					//”“¬÷…Ë÷√◊™ÀŸ-200RPM
//					data[0]=0x06; 
//					data[1]=0xFD; 
//					data[2]=0xDE; 
//					data[3]=0xE1; 
//				  Uart1ASendStr(data,4); 
//					//printf("\r\n PSB_TRIANGLE \r\n");   	
//				break; 
//				case PSB_CIRCLE:  	
//					//printf("\r\n PSB_CIRCLE \r\n");   		
//				break; 
//				case PSB_CROSS:   	
//					//printf("\r\n PSB_CROSS \r\n");    		
//				//◊Û¬÷…Ë÷√◊™ÀŸ50RPM
//					data[0]=0x06; 
//					data[1]=0x00; 
//					data[2]=0x88; 
//					data[3]=0x8E; 
//					Uart2ASendStr(data,4);
//					//”“¬÷…Ë÷√◊™ÀŸ-50RPM
//					data[0]=0x06; 
//					data[1]=0xFF; 
//					data[2]=0x78; 
//					data[3]=0x7D; 
//				  Uart1ASendStr(data,4); 
//				break; 
//				case PSB_SQUARE:  	
//					//printf("\r\n PSB_SQUARE \r\n");   		
//				break;
//				default:  break; 
//			}
//			
//		}else
//		{
//			//times++;
//			//if(times%10==0)LED=!LED;//…¡À∏LED,Ã· æœµÕ≥’˝‘⁄‘À––.
//			//if(times%50==0)printf("\r\n waiting the PS2 be pressed ("__DATE__ " - " __TIME__ ")\r\n");
//			//delay_ms(10);   
//		}
//			
//	}	 
}
 
