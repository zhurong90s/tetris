#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>

//ȫ�ֱ���
uint16_t CurX ,CurY ;
uint8_t GlNu ,GlNextNu;
uint8_t flage ,gameover ;
uint8_t Lift ,Right,Up,Down;
uint8_t point;
uint8_t * GameOver = "Game Over";
extern uint32_t SystemCoreClock; 

int main(void)
{
	/*<��ʼ������1�����������δ�ʱ������ĻˢΪ��ɫ>---------------------------------------*/
	uasrt1_init();
	tft_init();
	systick_init();
	ssd1289_set_bkclor(Black);

do{
	/*<��ʼ��ȫ�ֱ���>-------------------------------------------------------------------------*/
	CurX  = 5;		//box �ĵ�ǰ����
	CurY  = 65;
	flage = 0;		//������־λ	  	
	Lift  = 0;		//������־λ
	Right = 0;
	Down  = 0;
	Up  = 0;			
	GlNu = 16;		//��ǰbox���
	GlNextNu = 0;	//��һbox���
	gameover = 0;	//������־λ
	point = 0;
	/*<�ȴ�������Ĥ��ʼ��Ϸ���ȴ�ʱ��һֱ����GLNu>------------------------------------------*/
	while(flage == 0) GlNu = rand()%12;

	frame_init ();	 //��ʼ����ʾ
	show_box(CurX,CurY,GlNu);  //�����һ��box

	/*<�ȴ���Ϸ����>----------------------------------------------------------------------------*/
	while(gameover == 0) GlNextNu = rand()%19;    

	ssd1289_putchxs(90,30,Green,Black,GameOver);
	flage = 0;
	while(flage == 0) GlNu = rand()%12;
	ssd1289_set_bkclor(Black);
   }while(1);
	
}



