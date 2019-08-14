

#define Fix_Note_Address

#include "stm32f10x.h"
#include  "uip.h"

#include  "enc28j60.h"
#include  "httpd.h"

#define   NET_BASE_ADDR		0x6C100000
#define   NET_REG_ADDR			(*((volatile uint16_t *) NET_BASE_ADDR))
#define   NET_REG_DATA			(*((volatile uint16_t *) (NET_BASE_ADDR + 8)))

//#define FifoPointCheck

/*=============================================================================
  ϵͳȫ��ı���
  =============================================================================*/
#define ETH_ADDR_LEN			6

static unsigned char mymac[6] = {0x04,0x02,0x35,0x00,0x00,0x01};
static unsigned char myip[4] = {192,168,0,15};
// base url (you can put a DNS name instead of an IP addr. if you have
// a DNS server (baseurl must end in "/"):
static char baseurl[]="http://192.168.0.15/";
static unsigned int mywwwport =5005; // listen port for tcp/www (max range 1-254)
// or on a different port:
//static char baseurl[]="http://10.0.0.24:88/";
//static unsigned int mywwwport =88; // listen port for tcp/www (max range 1-254)
//
static unsigned int myudpport =1200; // listen port for udp




/*******************************************************************************
*	������: etherdev_init
*	��  ��: ��
*	��  ��: ��
*	��  ��: uIP �ӿں���,��ʼ������
*/
void etherdev_init(void)
{	 u8 i;

	/*initialize enc28j60*/
	enc28j60Init(mymac);					  
	//��IP��ַ��MAC��ַд����ԵĻ�����	ipaddr[] macaddr[]
	init_ip_arp_udp_tcp(mymac,myip,mywwwport);
	for (i = 0; i < 6; i++)
	{
		uip_ethaddr.addr[i] = mymac[i];
	}
    //ָʾ��״̬:0x476 is PHLCON LEDA(��)=links status, LEDB(��)=receive/transmit
    //enc28j60PhyWrite(PHLCON,0x7a4);
	//PHLCON��PHY ģ��LED ���ƼĴ���	
	enc28j60PhyWrite(PHLCON,0x0476);	
	enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz

}

/*******************************************************************************
*	������: etherdev_send
*	��  ��: p_char : ���ݻ�����
*			length : ���ݳ���
*	��  ��: ��
*	��  ��: uIP �ӿں���,����һ������
*/
void etherdev_send(uint8_t *p_char, uint16_t length)
{
	enc28j60PacketSend(length,p_char);

}

uint16_t etherdev_read(uint8_t *p_char)
{
	return enc28j60PacketReceive(1500,p_char);
}
////////////////////////////////////////////////////
void tcp_server_appcall(void){		
	switch(uip_conn->lport) 
	{
		case HTONS(80):
			httpd_appcall(); 
			break;
		case HTONS(1200):
		    tcp_demo_appcall(); 
			break;
	}
}


/*******************************************************************************
*	������: etherdev_poll
*	��  ��: ��
*	��  ��: ��
*	��  ��: uIP �ӿں���, ���ò�ѯ��ʽ����һ��IP��
*/
/*
                              etherdev_poll()

    This function will read an entire IP packet into the uip_buf.
    If it must wait for more than 0.5 seconds, it will return with
    the return value 0. Otherwise, when a full packet has been read
    into the uip_buf buffer, the length of the packet is returned.
*/
uint16_t etherdev_poll(void)
{
	uint16_t bytes_read = 0;
#if 0

	/* tick_count threshold should be 12 for 0.5 sec bail-out
		One second (24) worked better for me, but socket recycling
		is then slower. I set UIP_TIME_WAIT_TIMEOUT 60 in uipopt.h
		to counter this. Retransmission timing etc. is affected also. */
	while ((!(bytes_read = etherdev_read())) && (timer0_tick() < 12)) continue;

	timer0_reset();

#endif
	return bytes_read;
}
