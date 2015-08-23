  /**********************************************************
	*					����˹���麯���ļ�
	*					@R
	*********************************************************	
    */

#ifndef _TETRIS_H
#define _TETRIS_H

#include "app_tft.h"
#define Vertical_boxs	14 
#define Horizontal_boxs	9
#define MAX_BOX		19

//����˹������غ궨��
#define BUTTON_UP Magenta
#define BUTTON_DOWN Red

#define BSIZE 20   //�����С
#define SysStarX 5
#define SysStarY 5
#define SysEndX 285
#define SysEndY 185

#define able	1	//���ж��Ƿ��ƶ��������õ�
#define unable	0

//����board���
struct BOARD
{
	uint8_t var;
	uint16_t color;
};

//����box�ṹ��
struct SHAPE
{
	uint16_t box;
	uint16_t clor;
	uint8_t next;
};


void systick_init(void);
void frame_init (void);
void show_box(uint16_t x ,uint16_t y,uint8_t nu);
void eraser_box(uint16_t x ,uint16_t y,uint8_t nu);
uint8_t mov_able(uint16_t x ,uint16_t y,uint8_t nu);
uint8_t updata_board(uint16_t x ,uint16_t y,uint8_t nu);
void show_point(uint8_t point);
#endif
