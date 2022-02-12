#include "bsp_adc.h"

uint16_t ConvData;

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*��ʱ��*/
	RCC_APB2PeriphClockCmd(ADCx_PORT_CLK, ENABLE);
	/*���ò���*/
	GPIO_InitStruct.GPIO_Pin=ADCx_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;/*ģ������ģʽ*/
	/*д��Ĵ���*/
	GPIO_Init(ADCx_PORT, &GPIO_InitStruct);

}
static void ADCx_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	/*��ADCʱ��*/
	RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);
	/*���ò���*/
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;/*����ģʽ*/
	ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;/*����ת��*/
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;/*�����Ҷ���*/
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;/*��ʹ���ⲿӲ������*/
	ADC_InitStruct.ADC_NbrOfChannel=ADCx_CHx;/*ADC���ĸ�ͨ��*/
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;/*��ʹ������ɨ��*/
	/*������д��Ĵ���*/
	ADC_Init(ADC_x, &ADC_InitStruct);
	/*����ADC_CLK*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	/*����ͨ�����ã�ͨ��������˳�򣬲���ʱ��*/
	ADC_RegularChannelConfig(ADC_x, ADCx_CHx, 1, ADC_SampleTime_1Cycles5);
	/*ADCʹ��*/
	ADC_Cmd(ADC_x, ENABLE);
	/*У��ADC*/
	ADC_StartCalibration(ADC_x);
	while(ADC_GetCalibrationStatus(ADC_x)==RESET);
	/*DMA����*/
	ADC_DMACmd(ADC_x, ENABLE);
	/*ADC�����������ʼת��*/
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}
static void ADCx_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	/*��DMAʱ��*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /*����DMA����*/  
  DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)(&(ADC_x->DR));/*ADC�����ݼĴ���*/
  DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)(&ConvData);/*�洢����ַ*/
  DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;/*����ΪԴ*/
  DMA_InitStruct.DMA_BufferSize=1;/*���ݵĸ���Ϊ1*/
  DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;/*�����ַ������*/
  DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Disable;/*�洢����ַ������*/
  DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord; /*16λ���ݣ������ǰ���*/
  DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;/*������ָ����ѭ����ͣ�ط���*/
  DMA_InitStruct.DMA_Priority=DMA_Priority_High;/*DMA���ȼ���Ϊ��*/
  DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;/*��ʹ�ܴ洢�����洢��*/
	/*������д��Ĵ���*/
  DMA_Init(ADCx_DMA_CHx, &DMA_InitStruct);
	/*ʹ��DMA*/
  DMA_Cmd(ADCx_DMA_CHx, ENABLE);
}
void ADCx_Init(void)
{
	
	ADCx_GPIO_Config();
	ADCx_DMA_Config();
	ADCx_Config();
	
}


