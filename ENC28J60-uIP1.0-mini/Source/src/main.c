/****************************************************************************
* Copyright (C), 2010 奋斗嵌入式工作室 ourstm.5d6d.com
*
* 本例程在奋斗版STM32开发板MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com   
*
* 文件名: main.c
* 内容简述:
*		本例程移植uIP-1.0协议栈，演示开发板和PC间的TCP通信。自定义了一个简单的应用层
*	通信协议。本例程实现的功能有：
*		（1）通过PC机控制板子上的LED；
*		（2）实现了一个简单的Web服务器。
*	    奋斗版网卡模块ENC28J60，10M带宽。
*		本例程设置的缺省IP地址是 192.168.0.15，默认的TCP监听端口是1200。
*		开发板工作在TCP服务器模式。PC机工作在TCP客户端模式。
*	 	PC机上需要运行网络调试助手软件。
*	用户可以做如下测试：
*	（1）ping 试验
*		点击windows 开始-运行，执行cmd命令，然后在dos窗口输入 ping 192.168.0.15
*		应该看到如下结果：
*			Reply from 192.168.0.15: bytes=32 time<1ms TTL=128
*			Reply from 192.168.0.15: bytes=32 time<1ms TTL=128
*			Reply from 192.168.0.15: bytes=32 time<1ms TTL=128
*			Reply from 192.168.0.15: bytes=32 time<1ms TTL=128
*
*	（2）PC机控制开发板上的LED试验
*		运行网络调试助手软件，端口选择1200，服务器IP地址选择192.168.0.15， 协议类型选择TCP客户端，
    点击连接按钮，在命令输入窗口输入控制LED的命令字符串，然后点击发送，开发板上对应的LED灯会发生
	变化。
*		命令代码如下： (末尾无0x00和回车字符)
*		ledon 1     ----- 点亮LED1
*		ledoff 1    ----- 关闭LED1
*		ledon 2     ----- 点亮LED2
*		ledoff 2    ----- 关闭LED2
*		ledon 3     ----- 点亮LED3
*		ledoff 3    ----- 关闭LED3
*
    （3）打开IE浏览器，在地址栏输入http://192.168.0.15  可以看到基于uIP的Web测试页面
	     在对话框输入1-3,点确定按钮，可以控制相应的LED点亮。 
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2010-07-22 sun68  创建该文件
*
*/
/*
MINI与网络模块的连接关系 
PB8:  RST
PB12: SPI2 CS
PB13：SPI2 SCK
PB14: SPI2 MISO
PB15: SPI2 MOSI	   
*/
#include "stm32f10x.h"
#include <stdio.h>
#include "usart_printf.h"
#include "systick.h"
#include "SPI-fd.h"

/* 实现uip需要包含的h文件 */
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"
#include "enc28j60.h"
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

/*结束*/

#define EXAMPLE_NAME	"uIP1.0 TCP Demo"
#define EXAMPLE_DATE	"2010-07-22"

static void Init_fd(void);
static void DispLogo(void);
void InitNet(void);
void UserPro(void);
void UipPro(void);
void Delay(vu32 nCount);
/*******************************************************************************
*	函数名：main
*	输  入:
*	输  出:
*	功能说明：用户程序入口
*/
int main(void)
{
	Init_fd();	//板子资源的初始化
	DispLogo();		/* 显示例程Logo */
	SPI2_Init();
    
	InitNet();		/* 初始化网络设备以及UIP协议栈，配置IP地址 */

	/* 创建一个TCP监听端口和http监听端口，端口号为1200，80 */
	uip_listen(HTONS(1200));
	uip_listen(HTONS(5005));
	while (1)
	{
		UipPro();		/* 处理uip事件，必须插入到用户程序的循环体中 */


	}
}

/*******************************************************************************
*	函数名：UipPro
*	输  入:
*	输  出:
*	功能说明：uip协议栈的实现入口，必须被轮询处理。未用中断模式
*/
void UipPro(void)
{
	uint8_t i;
	static struct timer periodic_timer, arp_timer;
	static char timer_ok = 0;	/* fd*/

	/* 创建2个定时器，只用执行1次 */
	if (timer_ok == 0)
	{
		timer_ok = 1;
		timer_set(&periodic_timer, CLOCK_SECOND / 2);  /* 创建1个0.5秒的定时器 */
		timer_set(&arp_timer, CLOCK_SECOND * 10);	   /* 创建1个10秒的定时器 */
	}

	/*
		从网络设备读取一个IP包,返回数据长度 (非阻塞)
		这个地方没有使用DM9000AEP的中断功能，采用的是查询方式
	*/
	uip_len = tapdev_read();	/* uip_len 是在uip中定义的全局变量 */
	if(uip_len > 0)
	{
		/* 处理IP数据包(只有校验通过的IP包才会被接收) */
		if(BUF->type == htons(UIP_ETHTYPE_IP))
		{
			uip_arp_ipin();
			uip_input();
			/*
				当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
				需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)
			*/
			if (uip_len > 0)
			{
				uip_arp_out();
				tapdev_send();
			}
		}
		/* 处理arp报文 */
		else if (BUF->type == htons(UIP_ETHTYPE_ARP))
		{
			uip_arp_arpin();
			/*
				当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
				需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)
			*/
			if (uip_len > 0)
			{
				tapdev_send();
			}
		}
	}
	else if(timer_expired(&periodic_timer))	/* 0.5秒定时器超时 */
	{
		timer_reset(&periodic_timer);	/* 复位0.5秒定时器 */

		/* 轮流处理每个TCP连接, UIP_CONNS缺省是10个 */
		for(i = 0; i < UIP_CONNS; i++)
		{
			uip_periodic(i);	/* 处理TCP通信事件 */
			/*
				当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
				需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)
			*/
			if(uip_len > 0)
			{
				uip_arp_out();
				tapdev_send();
			}
		}

	#if UIP_UDP
		/* 轮流处理每个UDP连接, UIP_UDP_CONNS缺省是10个 */
		for(i = 0; i < UIP_UDP_CONNS; i++)
		{
			uip_udp_periodic(i);	/*处理UDP通信事件 */
			/* If the above function invocation resulted in data that
			should be sent out on the network, the global variable
			uip_len is set to a value > 0. */
			if(uip_len > 0)
			{
			uip_arp_out();
			tapdev_send();
			}
		}
	#endif /* UIP_UDP */

		/* 每隔10秒调用1次ARP定时器函数 */
		if (timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}

/*******************************************************************************
*	函数名：InitNet
*	输  入:
*	输  出:
*	功能说明：初始化网络硬件、UIP协议栈、配置本机IP地址
*/
void InitNet(void)
{
	uip_ipaddr_t ipaddr;


	tapdev_init();

	printf("uip_init\n\r");
	uip_init();

	printf("uip ip address : 192,168,0,15\n\r");
	uip_ipaddr(ipaddr, 192,168,0,15);
	uip_sethostaddr(ipaddr);

	printf("uip route address : 192,168,0,1\n\r");
	uip_ipaddr(ipaddr, 192,168,0,1);
	uip_setdraddr(ipaddr);

	printf("uip net mask : 255,255,255,0\n\r");
	uip_ipaddr(ipaddr, 255,255,255,0);
	uip_setnetmask(ipaddr);

}

void GPIO_Configuration(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE, ENABLE);
  	
	
	/* Configure the GPIO_LED pin */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	 GPIO_Init(GPIOA, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
      
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
  Delay(0xAFFF);					   
  GPIO_SetBits(GPIOB, GPIO_Pin_8 );		 	 	   //复位ENC28J60
  Delay(0xAFFF);		  	 	


}


void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }
/*******************************************************************************
	函数名：InitBoard
	输  入:
	输  出:
	功能说明：初始化硬件设备
*/
static void Init_fd(void)
{
	/*
		这个函数是ST库中的函数，函数实体在
		Libraries\CMSIS\Core\CM3\system_stm32f10x.c

		配置内部Flash接口，初始化PLL，配置系统频率
		系统时钟缺省配置为72MHz，你如果需要更改，则需要去修改相关的头文件中的宏定义
	 */
	SystemInit();

	/* 配置按键GPIO和LED GPIO */
	GPIO_Configuration();

	/* 配置串口 */
	USART2_Configuration();
    uart_init(57600);


	/* 配置systic作为1ms中断,这个函数在
	\Libraries\CMSIS\Core\CM3\core_cm3.h */
	SysTick_Config(SystemFrequency / 1000);
}

/*******************************************************************************
	函数名: DispLogo
	输  入:
	输  出:
	功能说明：显示例程Logo (通过串口打印到PC机的超级终端显示)
*/
static void DispLogo(void)
{
	/* 通过串口输出例程名和更新日期 */
	PrintfLogo(EXAMPLE_NAME, EXAMPLE_DATE);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

