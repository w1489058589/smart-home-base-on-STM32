/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	main.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.0
	*
	*	说明： 		接入onenet，上传数据和命令控制
	*
	*	修改记录：
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络协议层
#include <onenet.h>

//网络设备
#include <esp8266.h>

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "dht11.h"

//C库
#include <string.h>
#include <stdio.h>

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.96\",1883\r\n"
u8 temp;
u8 humi;

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

	Usart1_Init(115200);
	Usart2_Init(115200);

	while (DHT11_Init()) {
		UsartPrintf(USART_DEBUG, "DHT11 Error \r\n");
		DelayMs(1000);
	}
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
}

/*
************************************************************
*	函数名称：	main
*
*	函数功能：
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：
************************************************************
*/
int main(void)
{

	unsigned short timeCount = 0;	//发送间隔变量

	unsigned char *dataPtr = NULL;
	Hardware_Init();	//初始化外围硬件
	ESP8266_Init();
	while(1)
	{
		DHT11_Rst();
		// DHT11_Read_Data(&temp,&humi);//
		// UsartPrintf(USART_DEBUG,"P4****temp %d ,humi %df \r\n",temp,humi);
		// DelayXms(1000);
	}

}

