/**
*俄罗斯方块函数，
*zhurong90s@2013年10月14日13:27:42
*/
#include "stm32f10x.h"
struct BOARD Table_board[Vertical_boxs] [Horizontal_boxs];
struct SHAPE Boxs[MAX_BOX] =
{
	{0x88c0,Cyan,	1},
	{0xe800,Cyan,	2},
	{0xc440,Cyan,	3},
	{0x2e00,Cyan,	0},
	
	{0x44c0,Magenta,5},
	{0x8e00,Magenta,6},
	{0xc880,Magenta,7},
	{0xe200,Magenta,4},
	
	{0x8c40,Yellow,	9},
	{0x6c00,Yellow,	8},
	
	{0x4c80,Blue2,	11},
	{0xc600,Blue2,	10},
	
	{0x4e00,White,	13},
	{0x8c80,White,	14},
	{0xe400,White,	15},
	{0x4c40,White,	12},
	
	{0x8888,Red,	17},
	{0xf000,Red,	16},
	
	{0xcc00,Blue, 18}
};
/**
	函数名：systick_init
	功能：初始化systick定时器。
  */
void systick_init(void)
{
	//打开systick中断
    NVIC_SetPriority(SysTick_IRQn, 0x00);
	
	//设置systick时钟
	SysTick->LOAD = 0x0afc80;

	SysTick->CTRL = ( + SysTick_CTRL_TICKINT + SysTick_CTRL_CLKSOURCE );
}

/**
	函数名：frame_init
	功能：初始化显示，和触摸屏显示，开始systick时钟，显示point，。
	 
  */

void frame_init (void)
{	
	//整体表格绘制
	uint16_t cur_x,cur_y;
	uint8_t up[] = "Up",down[] = "Down",lift[] = "Lift",ringt[] = "Right";
	for(cur_x = SysStarX;cur_x < SysEndX;cur_x += BSIZE)
	{
		for(cur_y = SysStarY;cur_y < SysEndY;cur_y += BSIZE)
		{
			ssd1289_draw_line(cur_x        ,cur_y,cur_x        ,cur_y + BSIZE,Green);
			ssd1289_draw_line(cur_x + BSIZE,cur_y,cur_x + BSIZE,cur_y + BSIZE,Green);

			ssd1289_draw_line(cur_x,        cur_y,cur_x + BSIZE,cur_y        ,Green);
			ssd1289_draw_line(cur_x,cur_y + BSIZE,cur_x + BSIZE,cur_y + BSIZE,Green);	
		}
	}
	//绘制按键
	//第一个
	ssd1289_square_full(290,5,315,45,BUTTON_UP);
	ssd1289_putchs(295,10,Black,CoppyB,lift);

	//第二个
	ssd1289_square_full(290,68,315,108,BUTTON_UP);
	ssd1289_putchs(295,68,Black,CoppyB,ringt);

	//第三个
	ssd1289_square_full(290,131,315,171,BUTTON_UP);
	ssd1289_putchs(295,136,Black,CoppyB,down);

	//第四个
	ssd1289_square_full(290,194,315,234,BUTTON_UP);
	ssd1289_putchs(295,204,Black,CoppyB,up);

	//初始化board变量 】
	for(cur_x = 0;cur_x < Vertical_boxs;cur_x ++)
	{
		for(cur_y = 0;cur_y < Horizontal_boxs;cur_y ++)
		{
			Table_board[cur_x][cur_y].var = 0;
			Table_board[cur_x][cur_y].color = Black;
		}
				
	}
	//打开滴答定时器
	SysTick->CTRL |= SysTick_CTRL_ENABLE;
	//初始化分数区域
	show_point(0);		
}
 /**
 	函数名：show_box
	功能：在指定位置显示box。
	参数：(x,y)为box位置，nu为box的序列号。
   */
void show_box(uint16_t x ,uint16_t y,uint8_t nu)
{
	uint8_t i;
	uint16_t yt,xt;

	if((x<320)&&(y<240))
	{
		
		for(i=0;i<16;i++)
		{
			if(i<4)
			{
				if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
				{
					xt = x;
					yt = y+i*BSIZE;
					ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Boxs[nu].clor);	
				}
			}else if(i<8)
			{
				if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
				{
					xt = x + BSIZE;
					yt = y+(i-4)*BSIZE;
					ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Boxs[nu].clor);	
				}
			}else if(i<12)
			{
				if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
				{
					xt = x + BSIZE*2;
					yt = y+(i-8)*BSIZE;
					ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Boxs[nu].clor);	
				}
			}else if(i<16)
			{
				if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
				{
					xt = x + BSIZE*3;
					yt = y+(i-12)*BSIZE;
					ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Boxs[nu].clor);	
				}
			}
		}
	}	
} 
 /**
 	函数名：eraser_box
	功能：擦出指定位置的box。
	参数：(x,y)为box的位置。nu为box的编号。
   */
void eraser_box(uint16_t x ,uint16_t y,uint8_t nu)
{
	uint8_t i;
	uint16_t yt,xt;

	if((x<320)&&(y<240))
	{
		
		for(i=0;i<16;i++)
		{
			if(i<4)
			{
				if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
				{
					xt = x;
					yt = y+i*BSIZE;
					ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Black);	
				}
			}else if(i<8)
			{
				if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
				{
					xt = x + BSIZE;
					yt = y+(i-4)*BSIZE;
					ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Black);	
				}
			}else if(i<12)
			{
				if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
				{
					xt = x + BSIZE*2;
					yt = y+(i-8)*BSIZE;
					ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Black);	
				}
			}else if(i<16)
			{
				if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
				{
					xt = x + BSIZE*3;
					yt = y+(i-12)*BSIZE;
					ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Black);	
				}
			}
		}
	}	
}
/**
	函数名：mov_able
	功能：判断box是否能移动。
	参数：(x,y)移动后的位置，nu为box的序列号。
	      返回值为able，unable。	
  */
uint8_t mov_able(uint16_t x ,uint16_t y,uint8_t nu)
{
	uint8_t i;
	uint16_t yt = y,xt = x;
	
	for(i=0;i<16;i++)
	{
		if(i<4)
		{
			if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
			{
				xt = x;
				yt = y+i*BSIZE;
				if((xt<SysStarX)||(xt>=SysEndX)||(yt<SysStarY)||(yt>=SysEndY))
					return unable;
				if(Table_board[xt/BSIZE][yt/BSIZE].var == 1)
					return unable;
						
			}
		}else if(i<8)
		{
			if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
			{
				xt = x + BSIZE;
				yt = y+(i-4)*BSIZE;
				if((xt<SysStarX)||(xt>=SysEndX)||(yt<SysStarY)||(yt>=SysEndY))
					return unable;
				if(Table_board[xt/BSIZE][yt/BSIZE].var == 1)
					return unable;	
			}
		}else if(i<12)
		{
			if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
			{
				xt = x + BSIZE*2;
				yt = y+(i-8)*BSIZE;
				if((xt<SysStarX)||(xt>=SysEndX)||(yt<SysStarY)||(yt>=SysEndY))
					return unable;
				if(Table_board[xt/BSIZE][yt/BSIZE].var == 1)
					return unable;	
			}
		}else if(i<16)
		{
			if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
			{
				xt = x + BSIZE*3;
				yt = y+(i-12)*BSIZE;
				if((xt<SysStarX)||(xt>=SysEndX)||(yt<SysStarY)||(yt>=SysEndY))
					return unable;
				if(Table_board[xt/BSIZE][yt/BSIZE].var == 1)
					return unable;	
			}
		}
		
	}

	return able;
}

/**	函数名：updata_board
	功能： 把一个box添加到board上。消除一个满足条件的列。
		   刷新board显示。返回消掉的行数。
	参数：函数返回值为背刺调用消除的函数。（x，y）为要并入board的box。
		  nu为要并入box的序列号。
  */
uint8_t updata_board(uint16_t x ,uint16_t y,uint8_t nu)
{
	uint8_t i,j,row=0;
	uint16_t yt = y,xt = x;
	//把box添加到board上
	for(i=0;i<16;i++)
	{
		if(i<4)
		{
			if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
			{
				xt = x;
				yt = y+i*BSIZE;
				Table_board[xt/BSIZE][yt/BSIZE].var = 1;
				Table_board[xt/BSIZE][yt/BSIZE].color = Boxs[nu].clor; 					
			}
		}else if(i<8)
		{
			if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
			{
				xt = x + BSIZE;
				yt = y+(i-4)*BSIZE;
				Table_board[xt/BSIZE][yt/BSIZE].var = 1;
				Table_board[xt/BSIZE][yt/BSIZE].color = Boxs[nu].clor;	
			}
		}else if(i<12)
		{
			if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
			{
				xt = x + BSIZE*2;
				yt = y+(i-8)*BSIZE;
				Table_board[xt/BSIZE][yt/BSIZE].var = 1;
				Table_board[xt/BSIZE][yt/BSIZE].color = Boxs[nu].clor;	
			}
		}else if(i<16)
		{
			if((Boxs[nu].box>>(15 - i)&0x0001) == 0x0001)
			{
				xt = x + BSIZE*3;
				yt = y+(i-12)*BSIZE;
				Table_board[xt/BSIZE][yt/BSIZE].var = 1;
				Table_board[xt/BSIZE][yt/BSIZE].color = Boxs[nu].clor;	
			}
		}
	}
	//检查是否又可消除行
	//这段代码效率有点低
	//可以考虑全检测一遍，要消除的行数
	//在统一消除，有时间再尝试一下
	i = 14;
	do{
		 i--;
		 j=0;
		 for(j=0;j<9;j++)
		 {
		 	if(Table_board[i][j].var != 1) break;
		 }
		 //判断是否满行
		 if(j == 9)
		 {
		 	row++;
			xt=i;
		 	while(xt > 0)
			{
				for(yt=0;yt<9;yt++)
				{
					Table_board[xt][yt].var = Table_board[xt - 1][yt].var;
					Table_board[xt][yt].color = Table_board[xt - 1][yt].color;	
				}
				xt--;
			}
			
			for(yt=0;yt<9;yt++)
			{
				Table_board[0][yt].var = 0;
				Table_board[0][yt].color = Black;	
			}
		 	i=14;				
		 }

	}while(i != 0);


	for(i=0;i<14;i++)
	{
		for(j=0;j<9;j++)
		{
			xt=i*BSIZE + 5;
			yt=j*BSIZE + 5;
			ssd1289_square_full(xt,yt,xt+BSIZE,yt+BSIZE,Table_board[i][j].color);	
		}
	}
	
	return row;	
}
/**
	函数名：show_point
	功能：显示分数。
	参数：point为要显示的参数。
  */
void show_point(uint8_t point)
{
	
	ssd1289_putchs(69,190,Green,CoppyB,"Point");
	ssd1289_putchar(85,198,Green,Black,point/100 + 48);
	ssd1289_putchar(85,206,Green,Black,(point%100)/10 +48);
	ssd1289_putchar(85,214,Green,Black,point%10 + 48);
}

