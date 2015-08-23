  /**********************************************************
	*					俄罗斯方块函数文件
	*					@R
	*********************************************************	
    */
#include "stm32f10x.h"
#include <stdio.h>


/*
/函数名：uasrt1_init
/功能：初始化USART1，波特路9600，发送读取模式，禁止USART1发送接收中断。
/使用资源：		外设USART1。
/				PA时钟，USART1时钟。
/				TX——》PA9	 复用推挽输出。
/				RX——》PA10   上拉输入。
*/
void uasrt1_init(void)
{
	GPIO_InitTypeDef GPIOA_9; 
	GPIO_InitTypeDef GPIOA_10;
	USART_InitTypeDef USART_1;

	GPIOA_9.GPIO_Mode   = GPIO_Mode_AF_PP; 
	GPIOA_10.GPIO_Mode  = GPIO_Mode_IPU;
	GPIOA_9.GPIO_Pin    = GPIO_Pin_9;
	GPIOA_10.GPIO_Pin   = GPIO_Pin_10;
	GPIOA_9.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIOA_10.GPIO_Speed = GPIO_Speed_50MHz;

	USART_1.USART_BaudRate = 9600;
	USART_1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_1.USART_Mode = USART_Mode_Tx + USART_Mode_Rx;  
	USART_1.USART_Parity = USART_Parity_No;
	USART_1.USART_StopBits = USART_StopBits_1;
	USART_1.USART_WordLength = USART_WordLength_8b;
			 
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA+RCC_APB2Periph_USART1,ENABLE);
	GPIO_Init (GPIOA,&GPIOA_9);
	GPIO_Init (GPIOA,&GPIOA_10);
	USART_Init (USART1,&USART_1);
	USART_Cmd (USART1,ENABLE);
}
/*
/ 函数名：fputc&fgetc
/ 功能：支持stdio.h中的printf，getchar等函数，使用USART1，（scanf除外）。
/  注意：编译前，选中target options ——》target ——》USe MicroLIB。
*/
int fputc(int ch,FILE *f)
{
	while((USART1->SR&USART_SR_TXE) == 0);
	USART1->DR = (uint8_t)ch;
	return (int)ch;
}
int fgetc(FILE *f)
{
	while((USART1->SR&USART_SR_RXNE) == 0);
	return (int) (USART1->DR);

}
