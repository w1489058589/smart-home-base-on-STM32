/***************STM32F103C8T6**********************
* 文件名  ：DHT11.c
 * 描述    ：DHT11传感器
 * 备注    : DHT11温度湿度传感器
 * 接口    ：PA11-DATA

********************LIGEN*************************/
#include "dht11.h"
#include "delay.h"
#include "usart.h"
#define DT GPIO_Pin_0

uint8_t flag_dht11_read = 0;
//复位DHT11
void DHT11_Rst(void)	   
{
	DHT11_IO_OUT(); 	//SET OUTPUT
	DHT11_DQ_OUT(0); 	//使能GPIO_Pin_0，主机拉低18ms
	DelayXms(20);    //拉低至少18ms
	DHT11_DQ_OUT(1); 	//DQ=1
	DelayUs(30);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1：未检测到DHT11的存在
//返回0：存在
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80uS
	{
		retry++;
		DelayUs(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		DelayUs(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		DelayUs(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		DelayUs(1);
	}
	DelayUs(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
uint8_t DHT11_Read_Byte(void)    
{        
	uint8_t i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}


//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
		}
	}
	else return 1;
	return 0;	    
}
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在
uint8_t DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PG端口时钟
 	GPIO_InitStructure.GPIO_Pin = DT;				//PG11端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化I0口
 	GPIO_SetBits(GPIOA,DT);						//PG11输出高
			    
	DHT11_Rst();  //复位DHT11
	return DHT11_Check();//等待DHT11的回应
} 
//定时器2配置
//定时器读取温湿度
//优先级为1，子优先级为0
void TIM2_Configuration(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	//#1.使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//#2.配置基础参数
	TIM_TimeBaseStruct.TIM_Period = 999;
	TIM_TimeBaseStruct.TIM_Prescaler = 35999;
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
	//#3.使能定时器中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//#4.配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	//#5.启动定时器
	TIM_Cmd(TIM2, ENABLE);
}

//定时器回调函数
//当flag_dht11_read为1时进行温湿度读取，清清除标志位
void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		flag_dht11_read =1;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
