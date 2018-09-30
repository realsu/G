#include "WS2812B.h"
#include "sys.h"
/* �Դ� */                //B R G
unsigned long WsDat[nWs]={0xAA0000,0x770000,0x330000,0x110000,
													0x050000,0x000000,0x00AA00,0x007700,
													0x003300,0x001100,0x000500,0x000000,
													0x0000AA,0x000077,0x000033,0x000011,
													0x000005,0x000000};
/*
unsigned long WsDat[nWs]={0xFF0000,0xCC0000,0x990000,0x660000,0x330000,0x000000,0x000000,0x000000};
													0x00FF00,0x00CC00,0x009900,0x006600,0x003300,0x000000,0x000000,0x000000,
													0x0000FF,0x0000CC,0x000099,0x000066,0x000033,0x000000,0x000000,0x000000,
													0xFFFF00,0xCCCC00,0x999900,0x666600,0x333300,0x000000,0x000000,0x000000,
													0xFF00FF,0xCC00CC,0x990099,0x660066,0x330033,0x000000,0x000000,0x000000,
													0x00FFFF,0x00CCCC,0x009999,0x006666,0x003333,0x000000,0x000000,0x000000,
													0xFFFFFF,0xCCCCCC,0x999999,0x666666,0x333333,0x000000,0x000000,0x000000};
*/
/**************************************************************************************
* IO��ʼ������ֲʱ���޸ģ�
**************************************************************************************/
void WS_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	//�˿�ʱ�ӣ�ʹ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );	 

	// �˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				// PIN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// �����趨������ʼ�� 
}

/**************************
* �ڲ���ʱ
***************************/
void delay2us()
{
	unsigned char i;
	i=3;
	while(i)
	{
		i--;
	}
	
}
void delay05us()
{
	unsigned char i;
	i=1;
	while(i)
	{
		i--;
	}
}

/***************************
* ����һ����
****************************/
void TX0()  	{ PAout(7) = 1; delay05us(); PAout(7) = 0; delay2us(); } // ����0
void TX1()  	{ PAout(7) = 1; delay2us();  PAout(7) = 0; delay05us(); } // ����1
void WS_Reset() { PAout(7) = 0; delay_us(60);PAout(7) = 1; PAout(7) = 0;   }

/**************************************************************************************
* ����һ�ֽ�
**************************************************************************************/
void WS_Set1(unsigned long dat)
{
	unsigned char i;
	
	for(i=0; i<24; i++)
	{
		if(0x800000 == (dat & 0x800000) )	
			TX1();
		else						
			TX0();
		dat<<=1;							//����һλ
	}
}

/**************************************************************************************
* ���������ֽ�
**************************************************************************************/
void WS_SetAll()
{
	static uint8_t k;
	unsigned char j;
	unsigned long temp;
//	for(j=0; j<nWs; j++)
//	{
//		WS_Set1(WsDat[j]);  // j / 0
//	}
//	WS_Reset();

	{
		temp=WsDat[0];
		for(j=0;j<18;j++)
		{
			WsDat[j]=WsDat[j+1];
		}
		WsDat[17]=temp;
  }
	
}
/********************************************
* �����ֵ
********************************************/
unsigned char abs0(int num)
{
	if(num>0) return num;
	
	num = -num;
	return (unsigned char) num;
}

/***********************************************************************************
* ��ɫ�����㷨
* ��� <= 2
************************************************************************************/
u32 ColorToColor(unsigned long color0, unsigned long color1)
{
	unsigned char Red0, Green0, Blue0;  // ��ʼ��ԭɫ
	unsigned char Red1, Green1, Blue1;  // �����ԭɫ
	int			  RedMinus, GreenMinus, BlueMinus;	// ��ɫ�color1 - color0��
	unsigned char NStep; 							// ��Ҫ����
	float		  RedStep, GreenStep, BlueStep;		// ��ɫ����ֵ
	unsigned long color;							// ���ɫ
	unsigned char i,j;
	static uint32_t WsDatTemp;
	// �� �� �� ��ԭɫ�ֽ�
	Red0   = color0>>8;
	Green0 = color0>>16;
	Blue0  = color0;
	
	Red1   = color1>>8;
	Green1 = color1>>16;
	Blue1  = color1;
	
	// ������Ҫ���ٲ���ȡ��ֵ�����ֵ��
	RedMinus   = (Red1 - Red0); 
	GreenMinus = (Green1 - Green0); 
	BlueMinus  = (Blue1 - Blue0);
	
	NStep = ( abs0(RedMinus) > abs0(GreenMinus) ) ? abs0(RedMinus):abs0(GreenMinus);
	NStep = ( NStep > abs0(BlueMinus) ) ? NStep:abs0(BlueMinus);
	
	// �������ɫ����ֵ
	RedStep   = (float)RedMinus   / NStep;
	GreenStep = (float)GreenMinus / NStep;
	BlueStep  = (float)BlueMinus  / NStep;

	// ���俪ʼ
	for(i=0; i<NStep; i++)
	{
		Red1   = Red0   + (int)(RedStep   * i);
		Green1 = Green0 + (int)(GreenStep * i);
		Blue1  = Blue0  + (int)(BlueStep  * i);
		
		color  = Green1<<16 | Red1<<8 | Blue1; 	// �ϳ�  �̺���
		WsDatTemp = color;
		for(j=0; j<nWs; j++)
	{
		WS_Set1(WsDatTemp);  // j / 0
	}
	  WS_Reset();
		delay_ms(10);						// �����ٶ�
	}
	// �������
	
	return color;
}





