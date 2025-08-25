
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
#include "oled.h"
// #include "oledfont.h"
//C库
#include <string.h>
#include <stdio.h>
#include <stdint.h>


#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
uint8_t temp,humi;
void Hardware_Init(void);
void Refresh_Data(void);
void Display_Init(void);
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
	Display_Init();
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
	UsartPrintf(USART_DEBUG,"Connect MQTTs Server...\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO,"CONNECT"));
	DelayXms(500) ;
	UsartPrintf(USART_DEBUG,"Connect MQTT Server Success\r\n");
	while(OneNet_DevLink())
		DelayXms(500);
	OneNET_Subscribe();
	while(1)
	{
			DHT11_Read_Data(&temp,&humi);
			DelayMs(10);
			UsartPrintf(USART_DEBUG, "P4****temp %d ,humi %d\r\n",temp,humi);

			if(++timeCount >= 100)									//发送间隔5s
			{
				DHT11_Read_Data(&temp,&humi);

				//			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
				OneNet_SendData();									//发送数据

			timeCount = 0;
			ESP8266_Clear();
		}

			dataPtr = ESP8266_GetIPD(0);
			if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);

	 // Display_Init();
		 Refresh_Data();

	 	DelayMs(10);
	}

}
void Display_Init(void)
{
	OLED_Init();			//初始化OLED
	OLED_Clear();

	OLED_ShowCHinese(0,0,1);//温
	OLED_ShowCHinese(18,0,2);//度
	OLED_ShowCHinese(36,0,0);//：
	OLED_ShowCHinese(72,0,3);//℃


	OLED_ShowCHinese(0,3,4);//湿
	OLED_ShowCHinese(18,3,5);//度
	OLED_ShowCHinese(36,3,0);//：
	OLED_ShowString(72,3,"%",16);//%

	OLED_ShowCHinese(0,6,6);//台
	OLED_ShowCHinese(18,6,7);//灯
	OLED_ShowCHinese(36,6,0);//：

}
void Refresh_Data(void)
{
	char buf[3];
	sprintf(buf, "%2d", temp);
	OLED_ShowString(54,0,buf,16); //温度值

	sprintf(buf, "%2d", humi);
	OLED_ShowString(54,3,buf,16); //湿度值

	if(led_info.Led_Status) OLED_ShowCHinese(54,6,8);//亮
	else OLED_ShowCHinese(54,6,9);//灭

}

