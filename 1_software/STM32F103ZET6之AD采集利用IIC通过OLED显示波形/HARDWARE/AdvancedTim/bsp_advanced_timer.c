#include "bsp_advanced_timer.h"

static void AdvancedTim_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*��ʱ��*/
	TIMx_CH1_GPIO_CLK_CMD(TIMx_CH1_GPIO_CLK, ENABLE);
	TIMx_CH1N_GPIO_CLK_CMD(TIMx_CH1N_GPIO_CLK, ENABLE);
	TIMx_BKIN_GPIO_CLK_CMD(TIMx_BKIN_GPIO_CLK ,ENABLE);
	/*���ò���1*/
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=TIMx_CH1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	/*д����*/
	GPIO_Init(TIMx_CH1_GPIO_PORT, &GPIO_InitStruct);
	
	
	/*���ò���2*/
	GPIO_InitStruct.GPIO_Pin=TIMx_CH1N_GPIO_PIN;
	/*д����*/
	GPIO_Init(TIMx_CH1N_GPIO_PORT, &GPIO_InitStruct);
	
	
	/*���ò���3*/
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=TIMx_BKIN_GPIO_PIN;
	/*д����*/
	GPIO_Init(TIMx_BKIN_GPIO_PORT, &GPIO_InitStruct);
	
}

static void AdvancedTim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	/*��ʱ��*/
	RCC_APB2PeriphClockCmd(ADVANCED_TIM_CLK, ENABLE);
	/*����ʱ������*/
	TIM_TimeBaseInitStruct.TIM_Prescaler=ADVANCED_TIM_PSC;/*Ԥ��Ƶ����*/
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;/*���ϼ���*/
	TIM_TimeBaseInitStruct.TIM_Period=ADVANCED_TIM_ARR;/*����*/
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;/*Tdts:�����������ʱ���й�*/
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;/*��ʹ���ظ�������*/
	/*д����*/
	TIM_TimeBaseInit(ADVANCED_TIM, &TIM_TimeBaseInitStruct);
	
	/*��������ȽϵĲ���*/
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;/*PWMģʽһ*/
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;/*��ͨ��ʹ��*/
	TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;/*����ͨ��ʹ��*/
	TIM_OCInitStruct.TIM_Pulse=ADVANCED_TIM_CCR;/*ռ�ձ�*/
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;/*��ͨ���ߵ�ƽΪ��Ч*/
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;/*����ͨ���ߵ�ƽΪ��Ч*/
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Reset;/*ɲ��������״̬*/
	TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	/*д����*/
	TIM_OC1Init(ADVANCED_TIM, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(ADVANCED_TIM, TIM_OCPreload_Enable);
	
	/*��������ɲ���Ĵ���*/
	TIM_BDTRInitStruct.TIM_OSSRState=TIM_OSSRState_Enable;
	TIM_BDTRInitStruct.TIM_OSSIState=TIM_OSSIState_Enable;
	TIM_BDTRInitStruct.TIM_LOCKLevel=TIM_LOCKLevel_OFF;
	TIM_BDTRInitStruct.TIM_DeadTime=7;	/*����ʱ��97ns*/
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Enable;/*ʹ��ɲ������*/
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High;/*��ɲ��ͨ��Ϊ�ߵ�ƽʱֹͣ���*/
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(ADVANCED_TIM, &TIM_BDTRInitStruct);
	
	TIM_Cmd(ADVANCED_TIM, ENABLE);
	TIM_CtrlPWMOutputs(ADVANCED_TIM, ENABLE);
}

void AdvancedTim_Init(void)
{
		AdvancedTim_GPIO_Config();
		AdvancedTim_Config();
}
