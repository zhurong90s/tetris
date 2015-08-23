#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>

//全局变量
uint16_t CurX ,CurY ;
uint8_t GlNu ,GlNextNu;
uint8_t flage ,gameover ;
uint8_t Lift ,Right,Up,Down;
uint8_t point;
uint8_t * GameOver = "Game Over";
extern uint32_t SystemCoreClock; 

int main(void)
{
	/*<初始化串口1，触摸屏，滴答定时器，屏幕刷为黑色>---------------------------------------*/
	uasrt1_init();
	tft_init();
	systick_init();
	ssd1289_set_bkclor(Black);

do{
	/*<初始化全局变量>-------------------------------------------------------------------------*/
	CurX  = 5;		//box 的当前坐标
	CurY  = 65;
	flage = 0;		//触摸标志位	  	
	Lift  = 0;		//按键标志位
	Right = 0;
	Down  = 0;
	Up  = 0;			
	GlNu = 16;		//当前box编号
	GlNextNu = 0;	//下一box编号
	gameover = 0;	//结束标志位
	point = 0;
	/*<等待触摸屏膜后开始游戏，等待时，一直更新GLNu>------------------------------------------*/
	while(flage == 0) GlNu = rand()%12;

	frame_init ();	 //初始化显示
	show_box(CurX,CurY,GlNu);  //输出第一个box

	/*<等待游戏结束>----------------------------------------------------------------------------*/
	while(gameover == 0) GlNextNu = rand()%19;    

	ssd1289_putchxs(90,30,Green,Black,GameOver);
	flage = 0;
	while(flage == 0) GlNu = rand()%12;
	ssd1289_set_bkclor(Black);
   }while(1);
	
}



