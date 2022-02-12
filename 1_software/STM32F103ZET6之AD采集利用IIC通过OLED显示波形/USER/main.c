#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "bsp_adc.h"
#include "bsp_advanced_timer.h"
#include "bsp_i2c_gpio.h"
#include "OLED_I2C.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include <math.h>

/************************************************
 ʵ��0��ALIENTEK STM32F103�����幤��ģ��
 ע�⣬�����ֲ��еĵ����½�ʹ�õ�main�ļ�
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#define pi 3.1415926535
#define accur 0.015295//accur=18*3.3/4096��3.3/4096����ADC�������ȣ�18��Ϊ���ò���ת��һ���ܹ���ʾ���ʵ�λ�ӣ�
extern uint16_t ConvData;//ADC��������
extern unsigned char BMP1[];
int main()
{
	uint8_t x;
	LED_Init();

	OLED_Init();					 /* OLED��ʼ�� */
	ADCx_Init();
	AdvancedTim_Init();
	Before_State_Update(accur*ConvData);
	OLED_CLS();
	while(1)
	{
		for(x=0;x<128;x=(x+1)%128)
		{
			OLED_DrawWave(x,accur*ConvData);//���Ǹ������εĺ���
			//֮ǰд�˸����㺯������ʾ�Ĳ��β�������Ȼ���Ҿ͸���һ�»��㺯������������Ļ�Ͼ�������
		}
	}
}
