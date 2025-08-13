/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	led.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		BEEP��ʼ��������
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/


#include "stm32f10x.h"
#include "led.h"


LED_INFO led_info = {0};

void Led_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_13;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC, &gpio_initstruct);

	Led_Set(LED_ON);//影响初始化后LED的状态

}

void Led_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, status == LED_ON ? Bit_SET : Bit_RESET);//LED是否开启（LED_On），否则设置为（LED_OFF）
	
	led_info.Led_Status = status;

}
