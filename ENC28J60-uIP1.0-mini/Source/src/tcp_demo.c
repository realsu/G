

#include "tcp_demo.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "usart_printf.h"
static void aborted(void);
static void timedout(void);
static void closed(void);
static void connected(void);
static void newdata(void);
static void acked(void);
static void senddata(void);

static uint8_t test_data[2048];   /* 1K�������� */
u8 data[4];
//#define LED1_ON()	GPIO_SetBits(GPIOB,  GPIO_Pin_5);
#define LED1_ON()	GPIO_ResetBits(GPIOA,  GPIO_Pin_9);
#define LED2_ON()	GPIO_ResetBits(GPIOA,  GPIO_Pin_10);

//#define LED1_OFF()	GPIO_ResetBits(GPIOB,  GPIO_Pin_5);
#define LED1_OFF()	GPIO_SetBits(GPIOA,  GPIO_Pin_9);
#define LED2_OFF()	GPIO_SetBits(GPIOA,  GPIO_Pin_10);



/*******************************************************************************
*	������: tcp_demo_appcall
*	��  ��: ��
*	��  ��: ��
*	����˵��������һ���ص���������h�ļ��У����Ƕ��� UIP_APPCALL�ĺ���� tcp_demo_appcall
*		��uip�¼�����ʱ��UIP_APPCALL �����ᱻ���á�
*		���� : ��һ��TCP���ӱ�����ʱ�����µ����ݵ�������Ѿ���Ӧ��������Ҫ�ط����¼�
*

*/
void tcp_demo_appcall(void)
{
	if (uip_aborted())
	{
		//printf("uip_aborted!\r\n");
		aborted();
	}

	if (uip_timedout())
	{
		//printf("uip_timedout!\r\n");
		timedout();
	}

	if (uip_closed())
	{
		//printf("uip_closed!\r\n");
		closed();
	}

	if (uip_connected())
	{
		//printf("uip_connected!\r\n");
		connected();
	}

	if (uip_acked())
	{
		acked();
	}

	/* ���յ�һ���µ�TCP���ݰ���׼����Ҫ�������� */
	if (uip_newdata())
	{
		newdata();
	}

	/* ����Ҫ�ط��������ݵ�����ݰ��ʹ���ӽ���ʱ��֪ͨuip�������� */
	if (uip_rexmit() ||	uip_newdata() || uip_acked() ||	uip_connected() || uip_poll())
	{
		senddata();
	}
}

/*******************************************************************************
*	������: aborted
*	��  ��: ��
*	��  ��: ��
*	����˵������TCP�����쳣��ֹʱ�����ô˺�����
*/
static void aborted(void)
{
	;
}

static void timedout(void)
{
	;
}

static void closed(void)
{
	;
}

/*******************************************************************************
*	������: connected
*	��  ��: ��
*	��  ��: ��
*	����˵������TCP���ӽ���ʱ�����ô˺�����
*/
static void connected(void)
{  
    //unsigned char a[2]; 
	/*
		uip_conn�ṹ����һ��"appstate"�ֶ�ָ��Ӧ�ó����Զ���Ľṹ�塣
		����һ��sָ�룬��Ϊ�˱���ʹ�á�

		����Ҫ�ٵ���Ϊÿ��uip_conn�����ڴ棬����Ѿ���uip�з�����ˡ�
		��uip.c �� ����ش������£�
			struct uip_conn *uip_conn;
			struct uip_conn uip_conns[UIP_CONNS]; //UIP_CONNSȱʡ=10
		������1�����ӵ����飬֧��ͬʱ�����������ӡ�
		uip_conn��һ��ȫ�ֵ�ָ�룬ָ��ǰ��tcp��udp���ӡ�
	*/
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;

	//memset(test_data, 0x55, 2048);

	s->state = STATE_CMD;
	s->textlen = 0;

	s->textptr = "Connect STM32-FD Board Success!";
	s->textlen = strlen((char *)s->textptr);
	
	//senddata();
	
	//a[0]=0x0d; a[1]=0x0a;
	//s->textptr = a;
	//s->textlen = 2;

}

/*******************************************************************************
*	������: TCPCmd
*	��  ��: ��
*	��  ��: ��
*	����˵��������PC������������͵������������Ӧ�Ĵ���
*/
void TCP_Cmd(struct tcp_demo_appstate *s)
{
	uint8_t led;
	
	/* ����LED
		�﷨��ledon n  (n : 1 - 4) 
		���� �� 
		ledon 2 ��ʾ����LED2	
	*/
	if ((uip_len == 7) && (memcmp("ledon ", uip_appdata, 6) == 0))
	{
		led = ((uint8_t *)uip_appdata)[6]; /* ������LED��� */
		if (led == '1')
		{
			LED1_ON();
            //printf("on\r\n");
            
			s->textptr = "Motor On!";

            //���ݵ�ַA1
                data[0] = 0x80; 													
            //���ݸ�8λ
                data[1] = 0x00; 													
            //���ݵ�8λ
                data[2] = 0x80;										
            //����У���(A1+A2+A3)
                data[3] = 0x00;
            Uart1ASendStr(data,4);
            Uart2ASendStr(data,4);
		}
		else if (led == '2')
		{
			LED2_ON();
            //printf("led 2 on\r\n");
			s->textptr = "Led 2 On!";			
		}
		
		s->textlen = strlen((char *)s->textptr);
	}
	/* �ر�LED
		�﷨��ledoff n  (n : 1 - 4) 
		���� �� 
		ledon 2 ��ʾ����LED2	
	*/
	else if ((uip_len == 8) && (memcmp("ledoff ", uip_appdata, 7) == 0))
	{
		led = ((uint8_t *)uip_appdata)[7]; /* ������LED��� */
		if (led == '1')
		{
			LED1_OFF();
            //printf("off\r\n");
            //���ݵ�ַA1
            data[0] = 0x00; 													
            //���ݸ�8λ
            data[1] = 0x00; 													
            //���ݵ�8λ
            data[2] = 0x01;										
            //����У���(A1+A2+A3)
            data[3] = data[0] + data[1] + data[2];
            Uart1ASendStr(data,4);
            Uart2ASendStr(data,4);
			s->textptr = "Motor Off!";
		}
		else if (led == '2')
		{
			LED2_OFF();
            //printf("led 2 off\r\n");
			s->textptr = "Led 2 Off!";				
		}
				
		s->textlen = strlen((char *)s->textptr);
	}
	/* �������ݲ��� sendtest
		�﷨��sendtest
		���� ��
		ledon 2 ��ʾ����LED2
	*/
	else if ((uip_len == 6) && (memcmp("txtest", uip_appdata, 6) == 0))
	{
		s->state = STATE_TX_TEST;

		s->textptr = test_data;
		s->textlen = 1400;
	}
	else if ((uip_len == 6) && (memcmp("rxtest", uip_appdata, 6) == 0))
	{
		s->state = STATE_RX_TEST;
		s->textptr = "Ok";
		s->textlen = 2;
	}
	else
	{
		s->textptr = "Unknow Command!\r\n";
		s->textlen = strlen((char *)s->textptr);
	}
}

/*******************************************************************************
*	������: newdata
*	��  ��: ��
*	��  ��: ��
*	����˵�������յ��µ�TCP��ʱ�����ô˺�����׼�����ݣ�������ʱ�����͡�
*/
static void newdata(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	
	if (s->state == STATE_CMD)
	{
		//printf("uip_newdata!\r\n");
		TCP_Cmd(s);
	}
	else if (s->state == STATE_TX_TEST)	/* �ϴ�����״̬ */
	{
		/* �ڷ��Ͳ���״̬������յ�PC�����͵��������ݣ����˳�����״̬ */
		if ((uip_len == 1) && (((uint8_t *)uip_appdata)[0] == 'A'))
		{
			;/* �������� */
		}
		else
		{
			/* �˵�����״̬ */
	   		s->state = STATE_CMD;
			s->textlen = 0;
		}
	}
	else if (s->state == STATE_RX_TEST)	/* �´�����״̬ */
	{				
		if ((uip_len == 4) && (memcmp("stop", uip_appdata, 4) == 0))
		{
			/* �˵�����״̬ */
	   		s->state = STATE_CMD;
			s->textlen = 0;
		}
		else
		{
			static int sLen;

			sLen = uip_len;
			s->textptr = (uint8_t *)&sLen;		/* ���PC�����������Ӧ����յ������ݳ��� */
			s->textlen = 4;
		}
	}
}

/*******************************************************************************
*	������: acked
*	��  ��: ��
*	��  ��: ��
*	����˵���������͵�TCP���ɹ��ʹ�ʱ�����ô˺�����
*/
static void acked(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;

	switch(s->state)
	{
		case STATE_CMD:		 /* ������״̬ */
			s->textlen = 0;

			/* 
				ֻ������״̬��ӡ������Ϣ 
				���ⷢ�Ͳ���ʱ��Ӱ��ͨ���ٶ�		
			*/
			//printf("uip_acked!\r\n");
			break;

		case STATE_TX_TEST:
			s->textptr = test_data;	/* �������� */
			s->textlen = 1400;
			break;

		case STATE_RX_TEST:
			s->textlen = 0;
			break;
	}
}

/*******************************************************************************
*	������: senddata
*	��  ��: ��
*	��  ��: ��
*	����˵��������tcp���ݡ�
*/
static void senddata(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;

	if (s->textlen > 0)
	{
		/*
			��������������緢��TCP���ݰ�,
				s->textptr : ���͵����ݰ�������ָ��
				s->textlen �����ݰ��Ĵ�С����λ�ֽڣ�
		*/
		uip_send(s->textptr, s->textlen);
	}
}

/*******************************************************************************
*	������: uip_log
*	��  ��: m: �ַ���
*	��  ��: ��
*	����˵����uIP�ĵ��Դ�ӡ��䡣
*		����ͨ��ע�� #define UIP_CONF_LOGGING     ���رյ������
*/
void uip_log(char *m)
{
	printf("uIP log message: %s\r\n", m);
}

/*---------------------------------------------------------------------------*/
