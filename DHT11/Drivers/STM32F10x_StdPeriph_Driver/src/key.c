//软件
#include "stm32f10x.h"
//硬件
#include "key.h"
#include "delay.h"
#include "led.h"


void Key_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;//GPIO初始化
	EXTI_InitTypeDef exti_initstruct;//EXTI初始化
	NVIC_InitTypeDef nvic_initstruct;//NVIV初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//开启复用功能

	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;//输入上拉模式
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;//引脚
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_Init(GPIOA, &gpio_initstruct);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//EXTI配置

	exti_initstruct.EXTI_Line = EXTI_Line1;//LINE1
	exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;//上升沿触发
	exti_initstruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_Init(&exti_initstruct);

	nvic_initstruct.NVIC_IRQChannel = EXTI1_IRQn;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;//优先级
	nvic_initstruct.NVIC_IRQChannelSubPriority = 2;//子优先级
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&nvic_initstruct);

}

void EXTI1_IRQHandler(void)
{
	DelayXms(10);//按键按下后延时10s
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0)//判断是否按下
	{
		//判断LED状态
		if(led_info.Led_Status==LED_ON) Led_Set(LED_OFF);
		else Led_Set(LED_ON);
	}
	EXTI_ClearITPendingBit(EXTI_Line1);//清除标志位
}
