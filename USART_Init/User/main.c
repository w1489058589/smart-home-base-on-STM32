
//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
//C库
#include <string.h>
#include <stdio.h>
#include <stdint.h>
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
	 Usart1_Init();
	 Usart2_Init();
	 Delay_Init();

}

int main(void)
{
	Hardware_Init();
	while(1) {

		UsartPrintf(USART_DEBUG,"111111111\r\n");
		DelayXms(1000);
	}


}

