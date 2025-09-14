


//标准库
#include "stm32f10x.h"
#include "usart.h"
//C库
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/*
******************************************
*         函数名称：Usart2初始化
*
*         功能：初始化串口2
*
*         参数：波特率 = 115200;
*
*              Tx = PA2  ，Rx = PA3
*
*              中断触发源：RXNE
******************************************
*/
void Usart1_Init(void) {


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //初始化Tx引脚为PA9
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //初始化Rx引脚为PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //初始化Usart1
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);
    //使能串口
    USART_Cmd(USART1, ENABLE);
    //中断触发源为RXNE标志位非空
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //配置中断

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
}
/*
******************************************
*         函数名称：Usart2初始化
*
*         功能：初始化串口2
*
*         参数：波特率 = 115200;
*
*              Tx = PA2  ，Rx = PA3
*
*              中断触发源：RXNE
******************************************
*/
void Usart2_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    //初始化Tx引脚为PA2
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //初始化Rx引脚为PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //初始化Usart1
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);
    //使能串口
    USART_Cmd(USART2, ENABLE);
    //中断触发源为RXNE标志位非空
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    //配置中断
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}
/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

    unsigned short count = 0;
    for(; count < len; count++)
    {
        USART_SendData(USARTx, *str++);									//发送数据
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
    }

}
/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{
    unsigned char UsartPrintfBuf[296];//缓存区
    va_list ap;
    unsigned char *pStr = UsartPrintfBuf;
    va_start(ap, fmt);
    vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);	//格式化打印
    va_end(ap);
    while(*pStr != 0)//判断当前指向的字节是否为字符串结束符\0
    {
        USART_SendData(USARTx, *pStr++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
}
/*
************************************************************
*	函数名称：	USART1_IRQHandler
*
*	函数功能：	串口1收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：
************************************************************
*/
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //RXNE触发中断
    {
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
    }
}
/*
************************************************************
*	函数名称：	USART_DMA_Init
*
*	函数功能：	串口DMA配置
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：
************************************************************
*/
void USART_DMA_Init(void) {
	//使能DMA1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//复位DMA1通道
	DMA_DeInit(DMA1_Channel5);
	//配置DMA初始化结构体
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize = USART1_DMA_MAX_SIZE;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_M2M= DMA_M2M_Disable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;;
	//内存配置
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)usart1_rx_buffer;		//接收缓冲区地址
	DMA_InitStruct.DMA_MemoryDataSize = USART1_DMA_MAX_SIZE;			//存数据宽度：8位
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;				//存地址自增
	//外设配置
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);    //SART数据寄存器地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据宽度：8位
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设地址固定
	//应用DMA设置
	DMA_Init(DMA1_Channel5, &DMA_InitStruct);
	//使能DMA通道
	DMA_Cmd(DMA1_Channel5, ENABLE);
}
/*
************************************************************
*	函数名称：	USART1_IRQHandler
*
*	函数功能：	串口1DMA收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：
************************************************************
*/
void USART_DMA_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ReceiveData(USART1);
	 rx_idle_flag = 1;//
	}
}
/*
************************************************************
*	函数名称：	usart1_init_block_control_block
*
*	函数功能：	初始化接收控制块
*
*	入口参数：	无
*
*	返回参数：	无
*
************************************************************
*/
static void usart1_init_block_control_block(void) {
	//总接收字节计数器清零（从头开始存储）
	usart1_control_block.total_bytes_received = 0 ;
	//初始化读写指针到数据块数组起始位置
	usart1_control_block.current_write_block = &usart1_control_block.data_blocks[0];
	//设置数据块数组结束位置（最后一个元素的地址）
	usart1_control_block.blocks_array_end = &usart1_control_block.data_blocks[USART1_BLOCK_COUNT - 1];
	//设置第一个数据块的起始地址为接收缓冲区起始位置
	usart1_control_block.current_write_block->start_address = &usart1_rx_buffer[0];
}
/*
************************************************************
*	函数名称：	static void usart1_handle_idle_interrupt(void) {
*
*	函数功能：	串口1DMA收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：
************************************************************
*/
static void usart1_handle_idle_interrupt(void) {
	rx_idle_flag = 0;//清除空闲标志位
	//计算本次DMA所接收的实际字节数
	uint16_t received_bytes = USART1_DMA_MAX_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);
	//更新总接收字节数
	usart1_control_block.total_bytes_received += received_bytes;
	//设置当前数据块的结束地址（指向接收到的最后一个字节）
	usart1_control_block.current_write_block->end_address  =
		&usart1_rx_buffer[usart1_control_block.total_bytes_received - 1];
	//移动写指针到下一个数据块（循环队列）
	if (++usart1_control_block.current_write_block>=usart1_control_block.blocks_array_end) {
	//如果到达数组末尾，回绕到起始位置
		usart1_control_block.current_write_block = &usart1_control_block.data_blocks[0];
	//计算缓冲区剩余空间
	uint16_t remaining_space = USART1_DMA_MAX_SIZE - usart1_control_block.total_bytes_received;
	if (remaining_space >= received_bytes) {
		//剩余空间足够，设置下一个数据块的起始地址
		usart1_control_block.current_read_block->start_address =
			&usart1_rx_buffer[usart1_control_block.total_bytes_received];
	}
	else {
		//缓冲区不足，回绕到起始位置并重置计数器
		usart1_control_block.current_write_block->start_address = &usart1_rx_buffer[0];
		usart1_control_block.total_bytes_received = 0;
	}
	//重启DMA传输
	DMA_Cmd(DMA1_Channel5, DISABLE); //禁用DMA通道
	DMA1_Channel5 -> CNDTR = USART1_DMA_MAX_SIZE;// 设置传输数据量
	DMA1_Channel5->CMAR = (uint32_t)usart1_control_block.current_write_block->start_address; // 设置内存地址
	DMA_Cmd(DMA1_Channel5, ENABLE);//重新使能DMA通道
	}
}
