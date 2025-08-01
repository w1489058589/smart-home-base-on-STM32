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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		//��GPIOB��ʱ��
	

   	//��ֹJTAG����
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//����Ϊ���
	gpio_initstruct.GPIO_Pin = GPIO_Pin_13;						//����ʼ����Pin��
	gpio_initstruct.GPIO_Speed = GPIO_Speed_2MHz;				//�ɳ��ص����Ƶ��
	
	GPIO_Init(GPIOC, &gpio_initstruct);							//��ʼ��GPIO

	Led_Set(LED_OFF);											//��ʼ����ɺ󣬹رշ�����

}

/*
************************************************************
*	�������ƣ�	Beep_Set
*
*	�������ܣ�	����������
*
*	��ڲ�����	status�����ط�����
*
*	���ز�����	��
*
*	˵����		��-BEEP_ON		��-BEEP_OFF
************************************************************
*/
void Led_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, status == LED_ON ? Bit_SET : Bit_RESET);		//���status����BEEP_ON���򷵻�Bit_SET�����򷵻�Bit_RESET
	
	led_info.Led_Status = status;

}
