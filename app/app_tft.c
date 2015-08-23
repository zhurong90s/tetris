  /**********************************************************
	*					俄罗斯方块函数文件
	*					@R
	*********************************************************	
    */
#include "stm32f10x.h"
#include "ssd1289_font.h"

/**
  		-函数名:tft_init
  		-功能初始化tftlcd液晶配置
  		- 1.ads7843的spi通信与外部中断
		- 2.ssd1289的fsmc配置
  */
void tft_init(void)
{
	GPIO_InitTypeDef gpio_init;
	EXTI_InitTypeDef exit_init;
	NVIC_InitTypeDef nvic_exit_int;
	FSMC_NORSRAMInitTypeDef ssd1289_sarm_init;
	FSMC_NORSRAMTimingInitTypeDef sram_timing; 

	/**	<初始化ads7843 SPI通信及中断初始化>-------------------------------------------------------
	  */
	RCC_APB2PeriphClockCmd (ads7843_rcc,ENABLE);

	gpio_init.GPIO_Pin		= ads7843_cs;
	gpio_init.GPIO_Mode		= GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init (ads7843_cs_port,&gpio_init);		//初始化ads7843_cs推完输出
	ads7843_cs_1();								//拉高ads7843_cs

	gpio_init.GPIO_Pin		= ads7843_mo;
	GPIO_Init (ads7843_mo_port,&gpio_init);		//初始化ads7843_mo推完输出
	ads7843_mo_1();

	gpio_init.GPIO_Pin		= ads7843_sck;
	GPIO_Init (ads7843_sck_port,&gpio_init);	//初始化ads7843_sck推完输出
	ads7843_sck_0();

	gpio_init.GPIO_Pin		= ads7843_mi;
	gpio_init.GPIO_Mode		= GPIO_Mode_IPU;
	GPIO_Init (ads7843_mi_port,&gpio_init);	    //初始化ads7843_mi上拉输入

	gpio_init.GPIO_Pin		= ads7843_by;
	GPIO_Init (ads7843_by_port,&gpio_init);	    //初始化ads7843_by上拉输入

	gpio_init.GPIO_Pin		= ads7843_int;
	GPIO_Init (ads7843_int_port,&gpio_init);	//初始化ads7843_int上拉输入
	
	GPIO_EXTILineConfig(ads7843_int_sor_prot,ads7843_int_sor_pin); 
	exit_init.EXTI_Line		= ads7843_int_exit_lin;
	exit_init.EXTI_LineCmd	= ENABLE;
	exit_init.EXTI_Mode		= EXTI_Mode_Interrupt;
	exit_init.EXTI_Trigger	= EXTI_Trigger_Falling;
	EXTI_Init (&exit_init);							//初始化ads7843_int为下降沿

	nvic_exit_int.NVIC_IRQChannel	 = ads7843_int_exit_irqn;	 
	nvic_exit_int.NVIC_IRQChannelCmd = ENABLE;
	nvic_exit_int.NVIC_IRQChannelPreemptionPriority = 15;
	nvic_exit_int.NVIC_IRQChannelSubPriority		= 15;
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2); //配置系统中断为2组，
	NVIC_Init (&nvic_exit_int);						 //ads7843_int抢占优先级4，次优先级4

	/**	<初始化ssd1289 FSMC>--------------------------------------------------------------------
	  * GPIOD上连接的引脚：
	  * 	pd.00->D2,pd.01->D3,pd.04->NOW,pd.05->NWE,pd.08->D13,
	  * 	pd.09->D14,pd.10(D15), pd.14->D0,pd.15->D1。
	  * GPIOE上连接的引脚：
	  * 	pe.07->D4, pe.08->D5, pe.09->D6, pe.10->D7, pe.11->D8,
	  * 	pe.12->D9, pe.13->D10,pe.14->D11, pe.15->D12。
	  * GPIOF上连接的引脚：
	  * 	pf.00->A0。
	  * GPIOG上连接的引脚：
	  * 	pg.12->NE4(LCD/CS)。
	  *FSMC配置：
	  *		SRAM 1模式。BLANK1，BLOCK4 
	  * 	AddeSet :1
	  *		DataSet :2
	  *		详细如下。
	  */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|
							RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);

	gpio_init.GPIO_Pin		= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|
							  GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;
	gpio_init.GPIO_Mode		= GPIO_Mode_AF_PP;
	gpio_init.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init (GPIOD,&gpio_init);

	gpio_init.GPIO_Pin		= GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|
							  GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	gpio_init.GPIO_Mode		= GPIO_Mode_AF_PP;
	gpio_init.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init (GPIOE,&gpio_init);

	gpio_init.GPIO_Pin		= GPIO_Pin_0;										 
	GPIO_Init (GPIOF,&gpio_init);

	gpio_init.GPIO_Pin		= GPIO_Pin_12;										 
	GPIO_Init (GPIOG,&gpio_init);

	ssd1289_sarm_init.FSMC_ReadWriteTimingStruct = &sram_timing;
	ssd1289_sarm_init.FSMC_WriteTimingStruct= &sram_timing;
	FSMC_NORSRAMStructInit (&ssd1289_sarm_init);
	sram_timing.FSMC_AccessMode = FSMC_AccessMode_A;
	sram_timing.FSMC_AddressHoldTime = 0;
	sram_timing.FSMC_AddressSetupTime = 1;
	sram_timing.FSMC_BusTurnAroundDuration = 0;
	sram_timing.FSMC_CLKDivision = 0;
	sram_timing.FSMC_DataLatency = 0;
	sram_timing.FSMC_DataSetupTime = 2;
	ssd1289_sarm_init.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	ssd1289_sarm_init.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	ssd1289_sarm_init.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; 
	ssd1289_sarm_init.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	ssd1289_sarm_init.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	ssd1289_sarm_init.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	ssd1289_sarm_init.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	ssd1289_sarm_init.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	ssd1289_sarm_init.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	ssd1289_sarm_init.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	ssd1289_sarm_init.FSMC_WrapMode = FSMC_WrapMode_Disable; 
	ssd1289_sarm_init.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	ssd1289_sarm_init.FSMC_WriteOperation = FSMC_WriteOperation_Enable; 

	FSMC_NORSRAMInit(&ssd1289_sarm_init);
	FSMC_NORSRAMCmd (FSMC_Bank1_NORSRAM4,ENABLE); 
	/**<编程初始化ssd1289>--------------------------------------------------------------------
	  */
	 //电源控制
	 ssd1289_write_reg(0x0000,0x0001);		//打开晶振
	 ssd1289_write_reg(0x0003,0xA8A4);		
     ssd1289_write_reg(0x000C,0x0000);        
     ssd1289_write_reg(0x000D,0x080C);        
     ssd1289_write_reg(0x000E,0x2B00);        
     ssd1289_write_reg(0x001E,0x00B0);
	 //设置液晶驱动模块和模式        
	 ssd1289_write_reg(0x0001,0x2B3F);	
     ssd1289_write_reg(0x0002,0x0600);     	
     ssd1289_write_reg(0x0010,0x0000);     
     ssd1289_write_reg(0x0011,0x6070);		
     ssd1289_write_reg(0x0005,0x0000);     
     ssd1289_write_reg(0x0006,0x0000);     
     ssd1289_write_reg(0x0016,0xEF1C);     
     ssd1289_write_reg(0x0017,0x0003);     
     ssd1289_write_reg(0x0007,0x0233);		//0x0233       
     ssd1289_write_reg(0x000B,0x0000);     
     ssd1289_write_reg(0x000F,0x0000);		//扫描开始地址
     ssd1289_write_reg(0x0041,0x0000);     
     ssd1289_write_reg(0x0042,0x0000);     
     ssd1289_write_reg(0x0048,0x0000);     
     ssd1289_write_reg(0x0049,0x013F);     
     ssd1289_write_reg(0x004A,0x0000);     
     ssd1289_write_reg(0x004B,0x0000);     
     ssd1289_write_reg(0x0044,0xEF00);     
     ssd1289_write_reg(0x0045,0x0000);     
     ssd1289_write_reg(0x0046,0x013F);     
     ssd1289_write_reg(0x0030,0x0707);     
     ssd1289_write_reg(0x0031,0x0204);     
     ssd1289_write_reg(0x0032,0x0204);     
     ssd1289_write_reg(0x0033,0x0502);     
     ssd1289_write_reg(0x0034,0x0507);     
     ssd1289_write_reg(0x0035,0x0204);     
     ssd1289_write_reg(0x0036,0x0204);     
     ssd1289_write_reg(0x0037,0x0502);     
     ssd1289_write_reg(0x003A,0x0302);     
     ssd1289_write_reg(0x003B,0x0302);     
     ssd1289_write_reg(0x0023,0x0000);     
     ssd1289_write_reg(0x0024,0x0000);     
     ssd1289_write_reg(0x0025,0x8000);     
     ssd1289_write_reg(0x004e,0);        //列(X)首址0
     ssd1289_write_reg(0x004f,0);        //行(Y)首址0
	
}

/**
	函数名：ssd1289_write_reg
	功能：写ssd1289寄存器写入。
	参数：addr寄存器地址。
		  value要写入的数值。
  */

void ssd1289_write_reg(uint16_t addr,uint16_t value)
{
	*(uint16_t *)(0x6c000000) = addr;
	*(uint16_t *)(0x6c000002) = value;
}

/**
	函数名：ssd1289_set_cursor
	功能： 设置光标位置。
	参数：（x，y）分别为横坐标，与纵坐标。
  */

void ssd1289_set_cursor(uint16_t x,uint16_t y)
{
	ssd1289_write_reg(0x004e,y);	
	ssd1289_write_reg(0x004f,x);
}

/**
	函数名：ssd1289_set_point
	功能：设置（x，y）点的颜色
	参数：（x，y）坐标位置，clor要设置的颜色
  */
void ssd1289_set_point(uint16_t x,uint16_t y,uint16_t clor)
{
	if ( (x>=320)||(y>=240) ) return;

	*(uint16_t *)(0x6c000000) = 0x004e;	//设置y坐标位置
	*(uint16_t *)(0x6c000002) = y;
	*(uint16_t *)(0x6c000000) = 0x004f;	//设置x坐标位置
	*(uint16_t *)(0x6c000002) = x;

  	*(uint16_t *)(0x6c000000) = (uint16_t)(0x0022);	 //准备写数据
	*(uint16_t *)(0x6c000002) = clor;	
}
/**
	函数名：ssd1289_set_bkclor
	功能：设置液晶屏膜的背景颜色
	参数：bkclor想要设置背景的颜色。
	
  */
void ssd1289_set_bkclor(uint16_t bkclor)
{
	uint32_t i;

	*(uint16_t *)(0x6c000000) = 0x004e;	//设置y坐标位置
	*(uint16_t *)(0x6c000002) = 0x0000;
	*(uint16_t *)(0x6c000000) = 0x004f;	//设置x坐标位置
	*(uint16_t *)(0x6c000002) = 0x013f;
	*(uint16_t *)(0x6c000000) = 0x0022; //准备写数据
	for(i = 0;i < (76800);i++)
	{
		*(uint16_t *)(0x6c000002) = bkclor;		    //写入数据
	}
} 
/**
	函数名：ssd1289_putchar
	功能：在坐标(x,y)处，打印出字符串，分辨率8x16;
	参数：(x,y)坐标，chclor字符颜色，bkclor背景颜色，
		 ch要打印的字符串。
	注意,这个函数用到全局常量ascii_8x16，在头文件ssd1289中定义。	
  */
void ssd1289_putchar(uint16_t x,uint16_t y,uint16_t chclor,uint16_t bkclor,uint8_t ch)
{
	uint8_t yt,xt,cht;
	if(bkclor == CoppyB)
	{
  		for(xt = 0;xt < 16;xt++)
		{
			cht = ascii_8x16[(ch - 0x20)*16 + xt];
			for(yt = 0;yt < 8;yt++)
			{
			  if((cht>>(7-yt))&0x01 == 1)
			  	{
					ssd1289_set_point(x+xt,y+yt,chclor);	
				}
			} 	
		}
  	}
  	else{

		 for(xt = 0;xt < 16;xt++)
		{
			cht = ascii_8x16[(ch - 0x20)*16 + xt];
			for(yt = 0;yt < 8;yt++)
			{
			  if((cht>>(7-yt))&0x01 == 1)
			  	{
					ssd1289_set_point(x+xt,y+yt,chclor);	
				}
				else{
					
					ssd1289_set_point(x+xt,y+yt,bkclor);
				}
			} 	
		}

  	}
}
/**
	函数名：ssd1289_putchs
	功能：从(x,y)坐标开始，打印一串以'\0'结束的字符串,字体像素8x16。
	参数：(x,y)打印起始位置，chclor字体颜色，bkclor背景颜色，
	      ch要打印的字符串指针。
  */
void ssd1289_putchs(uint16_t x,uint16_t y,uint16_t chclor,uint16_t bkclor,uint8_t * ch)
{
	uint16_t i = 0;

	while(ch[i] != '\0')
	{
		if(y > 231) 
		{
			y  = 0;
			x += 16;
		}
		if( x > 304) x = 0;
		ssd1289_putchar(x,y,chclor,bkclor,ch[i]);
		i++;
		y += 8;
	}		
}
/**
	函数名：ssd1289_putchar
	功能：在坐标(x,y)处，打印出字符串，分辨率16x24;
	参数：(x,y)坐标，chclor字符颜色，bkclor背景颜色，
		 ch要打印的字符串。
	注意,这个函数用到全局常量ASCII_Table_16x24，在头文件ssd1289中定义。	
  */
void ssd1289_putchx(uint16_t x,uint16_t y,uint16_t chclor,uint16_t bkclor,uint8_t ch)
{
	uint8_t yt,xt;
	uint16_t cht;
	if(bkclor == CoppyB)
	{
  		for(xt = 0;xt < 24;xt++)
		{
			cht = ASCII_Table_16x24[(ch - 0x20)*24 + xt];
			for(yt = 0;yt < 16;yt++)
			{
			  if((cht>>yt)&0x01 == 1)
			  	{
					ssd1289_set_point(x+xt,y+yt,chclor);	
				}
			} 	
		}
  	}
  	else{

  		for(xt = 0;xt < 24;xt++)
		{
			cht = ASCII_Table_16x24[(ch - 0x20)*24 + xt];
			for(yt = 0;yt < 16;yt++)
			{
			  if((cht>>yt)&0x01 == 1)
			  	{
					ssd1289_set_point(x+xt,y+yt,chclor);	
				}
				else{
					
					ssd1289_set_point(x+xt,y+yt,bkclor);
				}
			} 	
		}

  	}
}
/**
	函数名：ssd1289_putchs
	功能：从(x,y)坐标开始，打印一串以'\0'结束的字符串,字体像素16x24。
	参数：(x,y)打印起始位置，chclor字体颜色，bkclor背景颜色，
	      ch要打印的字符串指针。
  */
void ssd1289_putchxs(uint16_t x,uint16_t y,uint16_t chclor,uint16_t bkclor,uint8_t * ch)
{
	uint16_t i = 0;

	while(ch[i] != '\0')
	{
		if(y > 225) 
		{
			y  = 0;
			x += 24;
		}
		if( x > 297) x = 0;
		ssd1289_putchx(x,y,chclor,bkclor,ch[i]);
		i++;
		y += 16;
	}		
}
/**
	函数名：ssd1289_square_full
	功能：填充一个矩形区域
	参数：(sx,sy),(ex,ey)分别为矩形区域起止坐标，clor要填充颜色。
  */
void ssd1289_square_full(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t clor)
{
	uint16_t i;
	if(ex>319) ex=319;
	if(ey>239) ey=239;
	if((sx +1 < ex)&&(sy+1 < ey))
	{
		for(sx=sx+1;sx<ex;sx++)
		{
			for(i=sy+1;i<ey;i++)
				ssd1289_set_point(sx,i,clor);
		}
	}
	
}

/**
	函数名：ssd1289_square_line
	功能：画一个矩形边框。
	参数：(sx,sy),(ex,ey)分别为矩形区域起止坐标，clor为边框的颜色。
  */
void ssd1289_square_line(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t clor)
{
	uint16_t t;
	if((sx < ex)&&(sy < ey))
	{
		if(ex>319) ex=319;
		if(ey>239) ey=239;
		t=sx;
		for(;sx<=ex;sx++)
		{
			ssd1289_set_point(sx,sy,clor);
			ssd1289_set_point(sx,ey,clor);
		}
		ex=t;
		for(;sy<=ey;sy++)
		{
			ssd1289_set_point(sx,sy,clor);
			ssd1289_set_point(ex,sy,clor);
		}
	}
	
}
/**

 */
void ssd1289_draw_line(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t clor)
{
	float k,yt,xt;
	if((ex>sx)&&(ey>sy))
	{
		yt =(float) (ey-sy);
		xt =(float) (ex-sx); 
		k =  yt/xt; 
		for(xt=sx;xt<=ex;xt++)
		{
			yt = (float)(k*(xt - sx) + sy);
			ssd1289_set_point(xt,yt,clor);	
		}
	}else if((ex>sx)&&(ey==sy))
	{
		for(;sx<=ex;sx++)
		{
			ssd1289_set_point(sx,sy,clor);	
		}	
	}else if((ex == sx)&&(ey>sy)) 
	{
		for(;sy<=ey;sy++)
		{
			ssd1289_set_point(sx,sy,clor);	
		}	
	}


}

/**
	函数名：ssd1289_draw
	功能：在（xs，ys）处开始打印一张图品
	参数：（xs，ys）图片打印位置，指针p指向要打印图片的位置。
	注意：可以用image2lcd直接取摸。水平，由左到右，由上到下。
		 p指针指向图片的前8个字节，为图的头信息。图片像素从p[8]
	     开始。图片的大小包含在头信息里。图解为:

		 【byte0】【byte1】【byte2】【byte3】【byte4】【byte5】【byte6】【byte7】
		typedef struct _HEADCOLOR
		{
		   unsigned char scan;
		   unsigned char gray;
		   unsigned short w;
		   unsigned short h;
		   unsigned char is565;
		   unsigned char rgb;
		}HEADCOLOR;
		
		scan: 扫描模式 

		Bit7: 0:自左至右扫描，1:自右至左扫描。 
		Bit6: 0:自顶至底扫描，1:自底至顶扫描。 
		Bit5: 0:字节内象素数据从高位到低位排列，1:字节内象素数据从低位到高位排列。 
		Bit4: 0:WORD类型高低位字节顺序与PC相同，1:WORD类型高低位字节顺序与PC相反。 
		Bit3~2: 保留。 
		Bit1~0: [00]水平扫描，[01]垂直扫描，[10]数据水平,字节垂直，[11]数据垂直,字节水平。 
		gray: 灰度值 
		   灰度值，1:单色，2:四灰，4:十六灰，8:256色，12:4096色，16:16位彩色，24:24位彩色，32:32位彩色。
		
		
		w: 图像的宽度。
		
		h: 图像的高度。

  */
void ssd1289_draw(uint16_t xs,uint8_t ys,const uint8_t * p)
{
	uint16_t *pwide = NULL;
	uint16_t *phigh = NULL;
	uint16_t xh,yw,xt,yt,* data;

	pwide = (uint16_t *)&p[2];
	phigh = (uint16_t *)&p[4];
	
	if((xs + *phigh) > 319) 
		xh = 319 - xs;
	else
	    xh = *phigh;
	
	if(ys + *pwide > 239)
		yw = 239 - ys;
	else
		yw = *pwide;

	data = (uint16_t *)&p[8]; 
	for(xt = 0;xt < xh;xt++)
	{
		
		for(yt = 0;yt < yw;yt++)
		{	
			ssd1289_set_point(xs+xt,ys+yt,*(data + xt*(*pwide) + yt));	
		}
	}
				
}
/**
	函数名：ssd1289_test
	功能：测试（x，y）坐标，函数按顺点亮屏幕为黄色。
	顺序: (0,0)-->(0,1)--> ~~ -->(0,239)
		  (1,0)-->(1,1)--> ~~ -->(1,239)
		  (2,0)-->(2,1)--> ~~ -->(2,239)
		   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  */
void ssd1289_test(void)
{
	uint32_t i,x,y,delay;
	ssd1289_set_cursor(0,0);
	*(uint16_t *)(0x6c000000) = (uint16_t)(0x0022);

	for(i = 0;i < (76800);i++)
	{
		*(uint16_t *)(0x6c000002) = 0x00;	
	}
	delay = 10;
	for(x=0;x<320;x++)
	{
		
		for(y=0;y<240;y++)
		{
			while(delay < 90010) delay ++;
			ssd1289_set_point (x,y,Yellow);	
			while(delay > 10) delay -- ;	
		}
	}

}
/**
   函数名：ads7843_send_cmd
   功能：发送ads7843的命令。
   参数：cmd要发送的参数。 
  */
void ads7843_send_cmd(uint8_t cmd)
{
	uint8_t i;
	//为开始通信做准备
	ads7843_cs_1();
	ads7843_mo_1();
	ads7843_sck_0();
	ads7843_cs_0();	

	for(i=0;i<8;i++)
	{
		if((cmd>>(7-i)&0x01) == 0x01)
			ads7843_mo_1();
		else
			ads7843_mo_0();
		
		ads7843_sck_1();
		ads7843_sck_0();
		
	}


	
}
/**
	函数名：ads7843_read_data
	功能：读取ads7843的坐标信息。
	参数：有一个无符号16位返回值。
  */
uint16_t ads7843_read_data(void)
{
	uint16_t data = 0;
	uint8_t i;

	//等待busy状态结束
	ads7843_sck_0();
	data = 0xff;
	while(data != 0 ) data--;


	for(i=0;i<12;i++)
	{
		ads7843_sck_1();
		if(ads7843_read_mi() == 1)
			data += 1;
		ads7843_sck_0();
		data = data << 1;	 			
	}
	return data;	
} 

/**
	函数名：ads7843_getxy
	功能：获取触摸点的坐标(x,y)。
	参数：通过指针的方式传入函数	
  */
void ads7843_getxy(uint16_t * x,uint16_t * y)
{
   uint16_t x_data[10],y_data[10],tmp;
   uint8_t i,j;
   for(i=0;i<10;i++)
   {
	   ads7843_send_cmd (0xd0);
	   y_data[i] = ads7843_read_data();
	   ads7843_send_cmd (0x90);
	   x_data[i] = ads7843_read_data();
   }


	//冒泡
	for(i=0;i<10;i++)
	{
		for(j=0;j<(9-i);j++)
		{
			if(x_data[j] > x_data[j+1])
				{
					tmp = x_data[j];
					x_data[j] = x_data[j+1];
					x_data[j+1] = tmp;	
				}
		}
	}

	for(i=0;i<10;i++)
	{
		for(j=0;j<(9-i);j++)
		{
			if(y_data[j] > y_data[j+1])
				{
					tmp = y_data[j];
					y_data[j] = y_data[j+1];
					y_data[j+1] = tmp;	
				}
		}
	}


	if(y_data[0] == 0)
	{
		i=1;
		while(y_data[i] == 0)
		{
			i++;
			if(i>9) break;
		}
		if(i<=9)
		{
			do{
				y_data[i-1] = y_data[i];
				i --;
			}while(i > 0);
		}


	}

	if(x_data[0] == 0)
	{
		i=1;
		while(x_data[i] == 0)
		{
			i++;
			if(i>9) break;
		}
		if(i<=9)
		{
			do{
				x_data[i-1] = x_data[i];
				i --;
			}while(i > 0);
		}


	}

// 	 printf("\r\n");
//	 for(i=0;i<10;i++)
//	 printf("%d  ",x_data[i]);
//	 printf("\r\n");
//	 for(i=0;i<10;i++)
//	 printf("%d  ",y_data[i]);
//	 printf("\r\n");

	*x = (x_data[7] + x_data[3] + x_data[4] + x_data[5] + x_data[6])/5;
	*y = (y_data[7] + y_data[3] + y_data[4] + y_data[5] + y_data[6])/5;

	if(*x>3850) 
		*x=3850;
	else if(*x<460) 
		*x=460;

	if(*y>3660) 
		*y=3660;
	else if(*y<295) 
		*y=295;

	*x = 320 - ((double)(*x - 460)/3390)*320;
	*y = ((double)(*y - 295)/3365)*240;
}
