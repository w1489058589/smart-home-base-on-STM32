#include "DHT11.h"
#include "delay.h"
#include "usart.h"


#define DT GPIO_Pin_0
void DHT11_Rst(void) {
    DHT11_IO_OUT();
    DHT11_DQ_OUT(0);//主机拉低，发起起始信号
    DelayXms(20);
    DHT11_DQ_OUT(1);//主机拉高，等待DHT11响应
    DelayUs(30);
}
/*
************************************************
*
* 函数名称：DHT11_CheckOut
*
* 函数功能：检测从机连接状态
*
* 入口参数：无
*
* 返回参数：1---超时   0---未超时
*
************************************************
*/
uint8_t DHT11_Check(void)
{
    uint8_t retry=0;
    DHT11_IO_IN();//SET INPUT
    while (DHT11_DQ_IN&&retry<100)  // 等待DHT11开始拉低总线（响应开始）
    {
        retry++;
        DelayUs(1);
    };
    if(retry>=100)return 1;
    else retry=0;
    while (!DHT11_DQ_IN&&retry<100)  // 等待DHT11开始拉低总线（响应结束）
    {
        retry++;
        DelayUs(1);
    };
    if(retry>=100)return 1;
    return 0;
}
/*
************************************************
*
* 函数名称：DHT11_Read_Bit
*
* 函数功能：开始读取数据
*
* 入口参数：无
*
* 返回参数：1---高电平   0---低电平
*
************************************************
*/
uint8_t DHT11_Read_Bit(void) {
    uint8_t retry = 0;
    while (DHT11_DQ_IN && retry < 100)//检测低电平
    {
        retry++;
        DelayUs(1);
    };
    retry = 0;
    while (!DHT11_DQ_IN && retry<100)//检测高电平
    {
        retry++;
        DelayUs(1);
    };
    DelayUs(40);
    if(DHT11_DQ_IN)return 1;
    else return 0;
}
/*
************************************************
*
* 函数名称：DHT11_Read_Byte
*
* 函数功能：读取一个字节数据
*
* 入口参数：无
*
* 返回参数：dat
*
************************************************
*/
uint8_t DHT11_Read_Byte(void){
    uint8_t i ,dat;
    for (i=0;i<8;i++) {
        dat <<= 1;
        dat|=DHT11_Read_Bit();
    }
    return dat;
}
/*
************************************************
*
* 函数名称：DHT11_Read_Data
*
* 函数功能：读取湿度，温度数据
*
* 入口参数：*temp--温度  *humi--湿度
*
* 返回参数：0
*
************************************************
*/
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi) {
    uint8_t Buf[5];
    uint8_t i;
    DHT11_Rst();//复位，主机发送开始信号
    if (DHT11_Check()==0) //检查DHT11通信连接是否超时
    {
        for (i=0;i<5;i++)//读取5个字节数据
        {
            Buf[i]=DHT11_Read_Byte();
        }
        if (Buf[0]+Buf[1]+Buf[2]+Buf[3] == Buf[4])//验证数据格式正确性
        {
            *humi = Buf[0]+Buf[1];//湿度数据
            *temp = Buf[2]+Buf[1];//温度数据
        }
    }
    else return 1;//DHT11通信连接超时
    return 0;
}
/*
************************************************
*
* 函数名称：DHT11_Init
*
* 函数功能：初始化DHT11
*
* 入口参数：无
*
* 返回参数：DHT11_Check
*
************************************************
*/
uint8_t DHT11_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_SetBits(GPIOA,DT) ;//拉高总线
    DHT11_Rst();
    return DHT11_Check();
}

