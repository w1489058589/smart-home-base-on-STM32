
//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "DHT11.h"
//C库
#include <string.h>
#include <stdio.h>
#include <stdint.h>
u8 temp;
u8 humi;

// extern uint8_t *temp,uint8_t *humi;

/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	Delay_Init();
	Led_Init();
	Key_Init();

	 Usart1_Init();
	 Usart2_Init();


	while (DHT11_Init()) {
		UsartPrintf(USART_DEBUG,"Connect DHT11 error\r\n");
		DelayXms(10);
	}
	UsartPrintf(USART_DEBUG,"Hardware Init Ok!\r\n");
}

int main(void)
{
	Hardware_Init();

	while(1) {
		//  DHT11_Rst() ;
		// DelayXms(1000);

		DHT11_Read_Data(&temp,&humi);//
		UsartPrintf(USART_DEBUG,"P4****temp %d ,humi %df  \r\n",temp,humi);
		DelayXms(1000);
	}


}

