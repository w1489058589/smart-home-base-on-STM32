

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//delayͷ�ļ�
#include "delay.h"


//��ʱϵ��
unsigned char UsCount = 0;
unsigned short MsCount = 0;


/*
************************************************************
*	函数名称：	Delay_Init
*
*	函数功能：	systick初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：
************************************************************
*/
void Delay_Init(void)
{

	SysTick->CTRL &= ~(1 << 2);		//ѡ��ʱ��ΪHCLK(72MHz)/8		103--9MHz
	
	UsCount = 9;					//΢�뼶��ʱϵ��
	
	MsCount = UsCount * 1000;		//���뼶��ʱϵ��

}

/*
************************************************************
*	函数名称：	DelayUs
*
*	函数功能：	微秒级延时
*
*	入口参数：	us：延时的时长
*
*	返回参数：	无
*
*	说明：		此时钟(21MHz)最大延时798915us
************************************************************
*/
void DelayUs(unsigned short us)
{

	unsigned int ctrlResult = 0;
	
	us &= 0x00FFFFFF;											//ȡ��24λ
	
	SysTick->LOAD = us * UsCount;								//װ������
	SysTick->VAL = 0;
	SysTick->CTRL = 1;											//ʹ�ܵ�������
	
	do
	{
		ctrlResult = SysTick->CTRL;
	}
	while((ctrlResult & 0x01) && !(ctrlResult & (1 << 16)));	//��֤�����С�����Ƿ񵹼�����0
	
	SysTick->CTRL = 0;											//�رյ�������
	SysTick->VAL = 0;

}

/*
************************************************************
*	函数名称：	DelayXms
*
*	函数功能：	毫秒级延时
*
*	入口参数：	ms：延时的时长
*
*	返回参数：	无
*
*	说明：
************************************************************
*/
void DelayXms(unsigned short ms)
{

	unsigned int ctrlResult = 0;
	
	if(ms == 0)
		return;
	
	ms &= 0x00FFFFFF;											//取低24位
	
	SysTick->LOAD = ms * MsCount;								//装载数据
	SysTick->VAL = 0;
	SysTick->CTRL = 1;										//使能倒计数器
	
	do
	{
		ctrlResult = SysTick->CTRL;
	}
	while((ctrlResult & 0x01) && !(ctrlResult & (1 << 16)));	//保证在运行,检查是否倒计数到0
	
	SysTick->CTRL = 0;											//关闭倒计数
	SysTick->VAL = 0;

}


/*
************************************************************
*	函数名称：	DelayMs
*
*	函数功能：	微秒级长延时
*
*	入口参数：	ms：延时的时长
*
*	返回参数：	无
*
*	说明：		多次调用DelayXms，做到长延时
************************************************************
*/
void DelayMs(unsigned short ms)
{

	unsigned char repeat = 0;
	unsigned short remain = 0;
	
	repeat = ms / 500;
	remain = ms % 500;
	
	while(repeat)
	{
		DelayXms(500);
		repeat--;
	}
	
	if(remain)
		DelayXms(remain);

}
