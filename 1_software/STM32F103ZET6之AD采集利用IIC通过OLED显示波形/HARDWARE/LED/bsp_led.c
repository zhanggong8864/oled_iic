#include "bsp_led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(LED_PORT_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=LED_PIN;
	GPIO_InitStruct.GPIO_Mode=LED_Output_Mode;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

