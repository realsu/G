/****************************************************************************

*
* 文件名: usart_printf.c
* 内容简述: 本模块实现printf和scanf函数重定向到串口1
*	实现重定向，只需要添加2个函数
		int fputc(int ch, FILE *f);
		int fgetc(FILE *f);
*


*
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "usart_printf.h"
#include "sys.h"
//#include "usart.h"
#include <stdarg.h>
/*******************************************************************************
	函数名：PrintfLogo
	输  入: 例程名称和例程最后更新日期
	输  出:
	功能说明：
*/
void PrintfLogo(char *strName, char *strDate)
{
	printf("*************************************************************\n\r");
	printf("* Example Name : %s\r\n", strName);
	printf("* Update Date  : %s\r\n", strDate);
	printf("* StdPeriph_Lib Version : V3.1.2\n\r");
	printf("* \n\r");
	printf("* Copyright ourstm.5d6d.com \r\n");
	printf("* QQ    : 9191274 \r\n");
	printf("* Email : sun68@qq.com \r\n");
	printf("*************************************************************\n\r");
}

/*******************************************************************************
	函数名：USART_Configuration
	输  入:
	输  出:
	功能说明：
	初始化串口硬件设备，未启用中断。
	配置步骤：
	(1)打开GPIO和USART的时钟
	(2)设置USART两个管脚GPIO模式
	(3)配置USART数据格式、波特率等参数
	(4)最后使能USART功能
*/
void USART2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* 第4步：配置USART参数
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	/* 第5步：使能 USART， 配置完毕 */
	USART_Cmd(USART2, ENABLE);

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */
}

/*******************************************************************************
	函数名：fputc
	输  入:
	输  出:
	功能说明：
	重定义putc函数，这样可以使用printf函数从串口1打印输出
*/
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART2, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/*******************************************************************************
	函数名：fputc
	输  入:
	输  出:
	功能说明：
	重定义getc函数，这样可以使用scanff函数从串口1输入数据
*/
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{}

	return (int)USART_ReceiveData(USART2);
}


//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	



//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}


/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/*
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}


void Uart1ASendStr (u8 *pucStr, u8 ulNum) 
{ 
	u8 i; 
for(i = 0;i<ulNum;i++) 
{ 
	     USART_SendData(USART1,*pucStr++);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );  

}  
} 

void Uart2ASendStr (u8 *pucStr, u8 ulNum) 
{ 
	u8 i; 
for(i = 0;i<ulNum;i++) 
{ 
	USART_SendData(USART2,*pucStr++);
	while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET );
     
}  
}

void COM_GIIMBot_DRV_Motor_Mode(unsigned char UARTx, unsigned char MotorOnOrOff)
{
		u8 data[4];
	//数据地址A1
		data[0] = 0x00; 													
	//数据高8位
		data[1] = 0x00; 													
	//数据底8位
		data[2] = MotorOnOrOff;										
	//数据校验和(A1+A2+A3)
		data[3] = data[0] + data[1] + data[2];
	
		if(UARTx == 1)
		{
			Uart1ASendStr(data,4);
		}
		else if (UARTx == 2)
		{	
			Uart2ASendStr(data,4);
		}
		else
		{
			Uart1ASendStr(data,4);
			Uart2ASendStr(data,4);
		}
}

void COM_GIIMBot_DRV_Mode_Choice(unsigned char UARTx, unsigned char ModeChoice)
{
	
		u8 data[4];
		//数据地址A1
		data[0] = ModeChoiceAddress; 													
		//数据高8位
		data[1] = 0x00; 													
		//数据底8位
		data[2] = ModeChoice;	
		//数据校验和(A1+A2+A3)
		data[3] = data[0] + data[1] + data[2];
		if(UARTx == 1)
		{
			Uart1ASendStr(data,4);
		}
		else if (UARTx == 2)
		{	
			Uart2ASendStr(data,4);
		}
		else
		{
			Uart1ASendStr(data,4);
			Uart2ASendStr(data,4);
		}
}








void COM_GIIMBot_DRV_Position_Mode(unsigned char UARTx, short Temp_PWM, int Temp_Position)
{
		u8 data_h[4];
		u8 data_l[4];
		u8 data_vel_limit[4];
		short PWM;
		PWM = Temp_PWM*16384/6000;
		//数据地址A1
		data_h[0] = PosModSetHighAddress; 													
		//数据高1-8位(number>>8)&0XFF; 
		data_h[1] = (u8)(Temp_Position >> 24);													
		//数据高2-8位
		data_h[2] = (u8)(Temp_Position >> 16);	
		//数据校验和(A1+A2+A3)
		data_h[3] = data_h[0] + data_h[1] + data_h[2];
		//数据地址A1
		data_l[0] = PosModSetLowAddress; 													
		//数据高1-8位(number>>8)&0XFF; 
		data_l[1] = (u8)(Temp_Position >> 8);													
		//数据高2-8位
		data_l[2] = (u8)Temp_Position;
		//数据校验和(A1+A2+A3)
		data_l[3] = data_l[0] + data_l[1] + data_l[2];
	
		data_vel_limit[0] = PosModSetVelLimitAddress;
		data_vel_limit[1] = (u8)(PWM >> 8);	
		data_vel_limit[2] = (u8)PWM;	
		data_vel_limit[3] = data_vel_limit[0] + data_vel_limit[1] + data_vel_limit[2];
	if( UARTx != 0)
		{
		switch(UARTx)
			{
				case UART1:  	
					Uart1ASendStr(data_h,4);
					Uart1ASendStr(data_l,4);
					Uart1ASendStr(data_vel_limit,4); 
					break;
				case UART2:  	
					Uart2ASendStr(data_h,4);
					Uart2ASendStr(data_l,4);	
					Uart2ASendStr(data_vel_limit,4);				
					break;
				case UART1_2:  	
					Uart1ASendStr(data_h,4);
					Uart2ASendStr(data_h,4);
					Uart1ASendStr(data_l,4);
					Uart2ASendStr(data_l,4); 	
					Uart1ASendStr(data_vel_limit,4);
					Uart2ASendStr(data_vel_limit,4);				
					break;
				default:  
					break; 	
			}
		}
}

#if EN_USART1_RX   //如果使能了接收
void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) 
    {
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 
        USART_RX_BUF[USART_RX_STA]=USART_ReceiveData(USART1);
		USART_RX_STA++; 
		if(USART_RX_BUF[0] == 0x80 || USART_RX_STA == USART_REC_LEN)    
		{
			if(USART_RX_BUF[1] == 0x00)                     
			{
				printf("%s\r\n",USART_RX_BUF);
				USART_RX_STA=0;                                   
			} 
			else
			{
				USART_RX_STA=0;  
			}
		}
	}
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//		{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
//		//printf("Res =USART_ReceiveData(USART1);\n\r");
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//			{
//                //printf("if((USART_RX_STA&0x8000)==0)\n\r");
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//				{
//                    //printf("if(USART_RX_STA&0x4000\n\r");
//				if(Res!=0x0a)
//                {
//                    USART_RX_STA=0;//接收错误,重新开始
//                    //printf("if(Res!=0x0a)\n\r");
//                }
//				else USART_RX_STA|=0x8000;	//接收完成了
//                    //printf("else USART_RX_STA|=0x8000;	\n\r");
//				}
//			else //还没收到0X0D
//				{	//printf("else no 0x0D\n\r");
//				if(Res==0x0d)
//                {
//                    USART_RX_STA|=0x4000;
//                    //printf("if(Res==0x0d)\n\r");
//                }
//				else
//					{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//                   // printf("USART_RX_BUF[USART_RX_STA&0X3FFF]=Res\n\r");
//					USART_RX_STA++;
//                    printf("%x",USART_RX_STA++);
//                    //printf("Res = %d",Res);
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
//     } 
    
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
}	 
#endif
