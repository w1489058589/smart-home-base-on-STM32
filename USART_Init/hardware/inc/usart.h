#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


#define USART_DEBUG		USART1		//���Դ�ӡ��ʹ�õĴ�����
extern unsigned char Buffer[295];
void Usart1_Init(void);
void Usart2_Init(void);
void Usart_Printf(USART_DEBUG, Buffer,unsigned int length);



void USART1_IRQHandLer(void);




#endif
