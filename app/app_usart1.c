  /**********************************************************
	*					����˹���麯���ļ�
	*					@R
	*********************************************************	
    */
#include "stm32f10x.h"
#include <stdio.h>


/*
/��������uasrt1_init
/���ܣ���ʼ��USART1������·9600�����Ͷ�ȡģʽ����ֹUSART1���ͽ����жϡ�
/ʹ����Դ��		����USART1��
/				PAʱ�ӣ�USART1ʱ�ӡ�
/				TX������PA9	 �������������
/				RX������PA10   �������롣
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
/ ��������fputc&fgetc
/ ���ܣ�֧��stdio.h�е�printf��getchar�Ⱥ�����ʹ��USART1����scanf���⣩��
/  ע�⣺����ǰ��ѡ��target options ������target ������USe MicroLIB��
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
