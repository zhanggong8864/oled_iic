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
 实验0：ALIENTEK STM32F103开发板工程模板
 注意，这是手册中的调试章节使用的main文件
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#define pi 3.1415926535
#define accur 0.015295//accur=18*3.3/4096（3.3/4096就是ADC采样精度，18是为了让波形转化一下能够显示在适当位子）
extern uint16_t ConvData;//ADC采样数据
extern unsigned char BMP1[];
int main()
{
	uint8_t x;
	LED_Init();

	OLED_Init();					 /* OLED初始化 */
	ADCx_Init();
	AdvancedTim_Init();
	Before_State_Update(accur*ConvData);
	OLED_CLS();
	while(1)
	{
		for(x=0;x<128;x=(x+1)%128)
		{
			OLED_DrawWave(x,accur*ConvData);//这是个画波形的函数
			//之前写了个画点函数，显示的波形不连续，然后我就改了一下画点函数，波形在屏幕上就连续了
		}
	}
}
