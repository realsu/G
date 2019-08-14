/****************************************************************************
* Copyright (C), 2010 �ܶ�Ƕ��ʽ������ ourstm.5d6d.com
*
* �������ڷܶ���STM32������MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com   
*
* �ļ���: main.c
* ���ݼ���:
*		��������ֲuIP-1.0Э��ջ����ʾ�������PC���TCPͨ�š��Զ�����һ���򵥵�Ӧ�ò�
*	ͨ��Э�顣������ʵ�ֵĹ����У�
*		��1��ͨ��PC�����ư����ϵ�LED��
*		��2��ʵ����һ���򵥵�Web��������
*	    �ܶ�������ģ��ENC28J60��10M����
*		���������õ�ȱʡIP��ַ�� 192.168.0.15��Ĭ�ϵ�TCP�����˿���1200��
*		�����幤����TCP������ģʽ��PC��������TCP�ͻ���ģʽ��
*	 	PC������Ҫ��������������������
*	�û����������²��ԣ�
*	��1��ping ����
*		���windows ��ʼ-���У�ִ��cmd���Ȼ����dos�������� ping 192.168.0.15
*		Ӧ�ÿ������½����
*			Reply from 192.168.0.15: bytes=32 time<1ms TTL=128
*			Reply from 192.168.0.15: bytes=32 time<1ms TTL=128
*			Reply from 192.168.0.15: bytes=32 time<1ms TTL=128
*			Reply from 192.168.0.15: bytes=32 time<1ms TTL=128
*
*	��2��PC�����ƿ������ϵ�LED����
*		���������������������˿�ѡ��1200��������IP��ַѡ��192.168.0.15�� Э������ѡ��TCP�ͻ��ˣ�
    ������Ӱ�ť�����������봰���������LED�������ַ�����Ȼ�������ͣ��������϶�Ӧ��LED�ƻᷢ��
	�仯��
*		����������£� (ĩβ��0x00�ͻس��ַ�)
*		ledon 1     ----- ����LED1
*		ledoff 1    ----- �ر�LED1
*		ledon 2     ----- ����LED2
*		ledoff 2    ----- �ر�LED2
*		ledon 3     ----- ����LED3
*		ledoff 3    ----- �ر�LED3
*
    ��3����IE��������ڵ�ַ������http://192.168.0.15  ���Կ�������uIP��Web����ҳ��
	     �ڶԻ�������1-3,��ȷ����ť�����Կ�����Ӧ��LED������ 
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2010-07-22 sun68  �������ļ�
*
*/
/*
MINI������ģ������ӹ�ϵ 
PB8:  RST
PB12: SPI2 CS
PB13��SPI2 SCK
PB14: SPI2 MISO
PB15: SPI2 MOSI	   
*/
#include "stm32f10x.h"
#include <stdio.h>
#include "usart_printf.h"
#include "systick.h"
#include "SPI-fd.h"

/* ʵ��uip��Ҫ������h�ļ� */
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"
#include "enc28j60.h"
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

/*����*/

#define EXAMPLE_NAME	"uIP1.0 TCP Demo"
#define EXAMPLE_DATE	"2010-07-22"

static void Init_fd(void);
static void DispLogo(void);
void InitNet(void);
void UserPro(void);
void UipPro(void);
void Delay(vu32 nCount);
/*******************************************************************************
*	��������main
*	��  ��:
*	��  ��:
*	����˵�����û��������
*/
int main(void)
{
	Init_fd();	//������Դ�ĳ�ʼ��
	DispLogo();		/* ��ʾ����Logo */
	SPI2_Init();
    
	InitNet();		/* ��ʼ�������豸�Լ�UIPЭ��ջ������IP��ַ */

	/* ����һ��TCP�����˿ں�http�����˿ڣ��˿ں�Ϊ1200��80 */
	uip_listen(HTONS(1200));
	uip_listen(HTONS(5005));
	while (1)
	{
		UipPro();		/* ����uip�¼���������뵽�û������ѭ������ */


	}
}

/*******************************************************************************
*	��������UipPro
*	��  ��:
*	��  ��:
*	����˵����uipЭ��ջ��ʵ����ڣ����뱻��ѯ����δ���ж�ģʽ
*/
void UipPro(void)
{
	uint8_t i;
	static struct timer periodic_timer, arp_timer;
	static char timer_ok = 0;	/* fd*/

	/* ����2����ʱ����ֻ��ִ��1�� */
	if (timer_ok == 0)
	{
		timer_ok = 1;
		timer_set(&periodic_timer, CLOCK_SECOND / 2);  /* ����1��0.5��Ķ�ʱ�� */
		timer_set(&arp_timer, CLOCK_SECOND * 10);	   /* ����1��10��Ķ�ʱ�� */
	}

	/*
		�������豸��ȡһ��IP��,�������ݳ��� (������)
		����ط�û��ʹ��DM9000AEP���жϹ��ܣ����õ��ǲ�ѯ��ʽ
	*/
	uip_len = tapdev_read();	/* uip_len ����uip�ж����ȫ�ֱ��� */
	if(uip_len > 0)
	{
		/* ����IP���ݰ�(ֻ��У��ͨ����IP���Żᱻ����) */
		if(BUF->type == htons(UIP_ETHTYPE_IP))
		{
			uip_arp_ipin();
			uip_input();
			/*
				������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
				��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)
			*/
			if (uip_len > 0)
			{
				uip_arp_out();
				tapdev_send();
			}
		}
		/* ����arp���� */
		else if (BUF->type == htons(UIP_ETHTYPE_ARP))
		{
			uip_arp_arpin();
			/*
				������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
				��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)
			*/
			if (uip_len > 0)
			{
				tapdev_send();
			}
		}
	}
	else if(timer_expired(&periodic_timer))	/* 0.5�붨ʱ����ʱ */
	{
		timer_reset(&periodic_timer);	/* ��λ0.5�붨ʱ�� */

		/* ��������ÿ��TCP����, UIP_CONNSȱʡ��10�� */
		for(i = 0; i < UIP_CONNS; i++)
		{
			uip_periodic(i);	/* ����TCPͨ���¼� */
			/*
				������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
				��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)
			*/
			if(uip_len > 0)
			{
				uip_arp_out();
				tapdev_send();
			}
		}

	#if UIP_UDP
		/* ��������ÿ��UDP����, UIP_UDP_CONNSȱʡ��10�� */
		for(i = 0; i < UIP_UDP_CONNS; i++)
		{
			uip_udp_periodic(i);	/*����UDPͨ���¼� */
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

		/* ÿ��10�����1��ARP��ʱ������ */
		if (timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}

/*******************************************************************************
*	��������InitNet
*	��  ��:
*	��  ��:
*	����˵������ʼ������Ӳ����UIPЭ��ջ�����ñ���IP��ַ
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
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	 GPIO_Init(GPIOA, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
      
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
  Delay(0xAFFF);					   
  GPIO_SetBits(GPIOB, GPIO_Pin_8 );		 	 	   //��λENC28J60
  Delay(0xAFFF);		  	 	


}


void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }
/*******************************************************************************
	��������InitBoard
	��  ��:
	��  ��:
	����˵������ʼ��Ӳ���豸
*/
static void Init_fd(void)
{
	/*
		���������ST���еĺ���������ʵ����
		Libraries\CMSIS\Core\CM3\system_stm32f10x.c

		�����ڲ�Flash�ӿڣ���ʼ��PLL������ϵͳƵ��
		ϵͳʱ��ȱʡ����Ϊ72MHz���������Ҫ���ģ�����Ҫȥ�޸���ص�ͷ�ļ��еĺ궨��
	 */
	SystemInit();

	/* ���ð���GPIO��LED GPIO */
	GPIO_Configuration();

	/* ���ô��� */
	USART2_Configuration();
    uart_init(57600);


	/* ����systic��Ϊ1ms�ж�,���������
	\Libraries\CMSIS\Core\CM3\core_cm3.h */
	SysTick_Config(SystemFrequency / 1000);
}

/*******************************************************************************
	������: DispLogo
	��  ��:
	��  ��:
	����˵������ʾ����Logo (ͨ�����ڴ�ӡ��PC���ĳ����ն���ʾ)
*/
static void DispLogo(void)
{
	/* ͨ����������������͸������� */
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

