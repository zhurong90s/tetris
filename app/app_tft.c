  /**********************************************************
	*					����˹���麯���ļ�
	*					@R
	*********************************************************	
    */
#include "stm32f10x.h"
#include "ssd1289_font.h"

/**
  		-������:tft_init
  		-���ܳ�ʼ��tftlcdҺ������
  		- 1.ads7843��spiͨ�����ⲿ�ж�
		- 2.ssd1289��fsmc����
  */
void tft_init(void)
{
	GPIO_InitTypeDef gpio_init;
	EXTI_InitTypeDef exit_init;
	NVIC_InitTypeDef nvic_exit_int;
	FSMC_NORSRAMInitTypeDef ssd1289_sarm_init;
	FSMC_NORSRAMTimingInitTypeDef sram_timing; 

	/**	<��ʼ��ads7843 SPIͨ�ż��жϳ�ʼ��>-------------------------------------------------------
	  */
	RCC_APB2PeriphClockCmd (ads7843_rcc,ENABLE);

	gpio_init.GPIO_Pin		= ads7843_cs;
	gpio_init.GPIO_Mode		= GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init (ads7843_cs_port,&gpio_init);		//��ʼ��ads7843_cs�������
	ads7843_cs_1();								//����ads7843_cs

	gpio_init.GPIO_Pin		= ads7843_mo;
	GPIO_Init (ads7843_mo_port,&gpio_init);		//��ʼ��ads7843_mo�������
	ads7843_mo_1();

	gpio_init.GPIO_Pin		= ads7843_sck;
	GPIO_Init (ads7843_sck_port,&gpio_init);	//��ʼ��ads7843_sck�������
	ads7843_sck_0();

	gpio_init.GPIO_Pin		= ads7843_mi;
	gpio_init.GPIO_Mode		= GPIO_Mode_IPU;
	GPIO_Init (ads7843_mi_port,&gpio_init);	    //��ʼ��ads7843_mi��������

	gpio_init.GPIO_Pin		= ads7843_by;
	GPIO_Init (ads7843_by_port,&gpio_init);	    //��ʼ��ads7843_by��������

	gpio_init.GPIO_Pin		= ads7843_int;
	GPIO_Init (ads7843_int_port,&gpio_init);	//��ʼ��ads7843_int��������
	
	GPIO_EXTILineConfig(ads7843_int_sor_prot,ads7843_int_sor_pin); 
	exit_init.EXTI_Line		= ads7843_int_exit_lin;
	exit_init.EXTI_LineCmd	= ENABLE;
	exit_init.EXTI_Mode		= EXTI_Mode_Interrupt;
	exit_init.EXTI_Trigger	= EXTI_Trigger_Falling;
	EXTI_Init (&exit_init);							//��ʼ��ads7843_intΪ�½���

	nvic_exit_int.NVIC_IRQChannel	 = ads7843_int_exit_irqn;	 
	nvic_exit_int.NVIC_IRQChannelCmd = ENABLE;
	nvic_exit_int.NVIC_IRQChannelPreemptionPriority = 15;
	nvic_exit_int.NVIC_IRQChannelSubPriority		= 15;
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2); //����ϵͳ�ж�Ϊ2�飬
	NVIC_Init (&nvic_exit_int);						 //ads7843_int��ռ���ȼ�4�������ȼ�4

	/**	<��ʼ��ssd1289 FSMC>--------------------------------------------------------------------
	  * GPIOD�����ӵ����ţ�
	  * 	pd.00->D2,pd.01->D3,pd.04->NOW,pd.05->NWE,pd.08->D13,
	  * 	pd.09->D14,pd.10(D15), pd.14->D0,pd.15->D1��
	  * GPIOE�����ӵ����ţ�
	  * 	pe.07->D4, pe.08->D5, pe.09->D6, pe.10->D7, pe.11->D8,
	  * 	pe.12->D9, pe.13->D10,pe.14->D11, pe.15->D12��
	  * GPIOF�����ӵ����ţ�
	  * 	pf.00->A0��
	  * GPIOG�����ӵ����ţ�
	  * 	pg.12->NE4(LCD/CS)��
	  *FSMC���ã�
	  *		SRAM 1ģʽ��BLANK1��BLOCK4 
	  * 	AddeSet :1
	  *		DataSet :2
	  *		��ϸ���¡�
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
	/**<��̳�ʼ��ssd1289>--------------------------------------------------------------------
	  */
	 //��Դ����
	 ssd1289_write_reg(0x0000,0x0001);		//�򿪾���
	 ssd1289_write_reg(0x0003,0xA8A4);		
     ssd1289_write_reg(0x000C,0x0000);        
     ssd1289_write_reg(0x000D,0x080C);        
     ssd1289_write_reg(0x000E,0x2B00);        
     ssd1289_write_reg(0x001E,0x00B0);
	 //����Һ������ģ���ģʽ        
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
     ssd1289_write_reg(0x000F,0x0000);		//ɨ�迪ʼ��ַ
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
     ssd1289_write_reg(0x004e,0);        //��(X)��ַ0
     ssd1289_write_reg(0x004f,0);        //��(Y)��ַ0
	
}

/**
	��������ssd1289_write_reg
	���ܣ�дssd1289�Ĵ���д�롣
	������addr�Ĵ�����ַ��
		  valueҪд�����ֵ��
  */

void ssd1289_write_reg(uint16_t addr,uint16_t value)
{
	*(uint16_t *)(0x6c000000) = addr;
	*(uint16_t *)(0x6c000002) = value;
}

/**
	��������ssd1289_set_cursor
	���ܣ� ���ù��λ�á�
	��������x��y���ֱ�Ϊ�����꣬�������ꡣ
  */

void ssd1289_set_cursor(uint16_t x,uint16_t y)
{
	ssd1289_write_reg(0x004e,y);	
	ssd1289_write_reg(0x004f,x);
}

/**
	��������ssd1289_set_point
	���ܣ����ã�x��y�������ɫ
	��������x��y������λ�ã�clorҪ���õ���ɫ
  */
void ssd1289_set_point(uint16_t x,uint16_t y,uint16_t clor)
{
	if ( (x>=320)||(y>=240) ) return;

	*(uint16_t *)(0x6c000000) = 0x004e;	//����y����λ��
	*(uint16_t *)(0x6c000002) = y;
	*(uint16_t *)(0x6c000000) = 0x004f;	//����x����λ��
	*(uint16_t *)(0x6c000002) = x;

  	*(uint16_t *)(0x6c000000) = (uint16_t)(0x0022);	 //׼��д����
	*(uint16_t *)(0x6c000002) = clor;	
}
/**
	��������ssd1289_set_bkclor
	���ܣ�����Һ����Ĥ�ı�����ɫ
	������bkclor��Ҫ���ñ�������ɫ��
	
  */
void ssd1289_set_bkclor(uint16_t bkclor)
{
	uint32_t i;

	*(uint16_t *)(0x6c000000) = 0x004e;	//����y����λ��
	*(uint16_t *)(0x6c000002) = 0x0000;
	*(uint16_t *)(0x6c000000) = 0x004f;	//����x����λ��
	*(uint16_t *)(0x6c000002) = 0x013f;
	*(uint16_t *)(0x6c000000) = 0x0022; //׼��д����
	for(i = 0;i < (76800);i++)
	{
		*(uint16_t *)(0x6c000002) = bkclor;		    //д������
	}
} 
/**
	��������ssd1289_putchar
	���ܣ�������(x,y)������ӡ���ַ������ֱ���8x16;
	������(x,y)���꣬chclor�ַ���ɫ��bkclor������ɫ��
		 chҪ��ӡ���ַ�����
	ע��,��������õ�ȫ�ֳ���ascii_8x16����ͷ�ļ�ssd1289�ж��塣	
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
	��������ssd1289_putchs
	���ܣ���(x,y)���꿪ʼ����ӡһ����'\0'�������ַ���,��������8x16��
	������(x,y)��ӡ��ʼλ�ã�chclor������ɫ��bkclor������ɫ��
	      chҪ��ӡ���ַ���ָ�롣
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
	��������ssd1289_putchar
	���ܣ�������(x,y)������ӡ���ַ������ֱ���16x24;
	������(x,y)���꣬chclor�ַ���ɫ��bkclor������ɫ��
		 chҪ��ӡ���ַ�����
	ע��,��������õ�ȫ�ֳ���ASCII_Table_16x24����ͷ�ļ�ssd1289�ж��塣	
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
	��������ssd1289_putchs
	���ܣ���(x,y)���꿪ʼ����ӡһ����'\0'�������ַ���,��������16x24��
	������(x,y)��ӡ��ʼλ�ã�chclor������ɫ��bkclor������ɫ��
	      chҪ��ӡ���ַ���ָ�롣
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
	��������ssd1289_square_full
	���ܣ����һ����������
	������(sx,sy),(ex,ey)�ֱ�Ϊ����������ֹ���꣬clorҪ�����ɫ��
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
	��������ssd1289_square_line
	���ܣ���һ�����α߿�
	������(sx,sy),(ex,ey)�ֱ�Ϊ����������ֹ���꣬clorΪ�߿����ɫ��
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
	��������ssd1289_draw
	���ܣ��ڣ�xs��ys������ʼ��ӡһ��ͼƷ
	��������xs��ys��ͼƬ��ӡλ�ã�ָ��pָ��Ҫ��ӡͼƬ��λ�á�
	ע�⣺������image2lcdֱ��ȡ����ˮƽ�������ң����ϵ��¡�
		 pָ��ָ��ͼƬ��ǰ8���ֽڣ�Ϊͼ��ͷ��Ϣ��ͼƬ���ش�p[8]
	     ��ʼ��ͼƬ�Ĵ�С������ͷ��Ϣ�ͼ��Ϊ:

		 ��byte0����byte1����byte2����byte3����byte4����byte5����byte6����byte7��
		typedef struct _HEADCOLOR
		{
		   unsigned char scan;
		   unsigned char gray;
		   unsigned short w;
		   unsigned short h;
		   unsigned char is565;
		   unsigned char rgb;
		}HEADCOLOR;
		
		scan: ɨ��ģʽ 

		Bit7: 0:��������ɨ�裬1:��������ɨ�衣 
		Bit6: 0:�Զ�����ɨ�裬1:�Ե�����ɨ�衣 
		Bit5: 0:�ֽ����������ݴӸ�λ����λ���У�1:�ֽ����������ݴӵ�λ����λ���С� 
		Bit4: 0:WORD���͸ߵ�λ�ֽ�˳����PC��ͬ��1:WORD���͸ߵ�λ�ֽ�˳����PC�෴�� 
		Bit3~2: ������ 
		Bit1~0: [00]ˮƽɨ�裬[01]��ֱɨ�裬[10]����ˮƽ,�ֽڴ�ֱ��[11]���ݴ�ֱ,�ֽ�ˮƽ�� 
		gray: �Ҷ�ֵ 
		   �Ҷ�ֵ��1:��ɫ��2:�Ļң�4:ʮ���ң�8:256ɫ��12:4096ɫ��16:16λ��ɫ��24:24λ��ɫ��32:32λ��ɫ��
		
		
		w: ͼ��Ŀ�ȡ�
		
		h: ͼ��ĸ߶ȡ�

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
	��������ssd1289_test
	���ܣ����ԣ�x��y�����꣬������˳������ĻΪ��ɫ��
	˳��: (0,0)-->(0,1)--> ~~ -->(0,239)
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
   ��������ads7843_send_cmd
   ���ܣ�����ads7843�����
   ������cmdҪ���͵Ĳ����� 
  */
void ads7843_send_cmd(uint8_t cmd)
{
	uint8_t i;
	//Ϊ��ʼͨ����׼��
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
	��������ads7843_read_data
	���ܣ���ȡads7843��������Ϣ��
	��������һ���޷���16λ����ֵ��
  */
uint16_t ads7843_read_data(void)
{
	uint16_t data = 0;
	uint8_t i;

	//�ȴ�busy״̬����
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
	��������ads7843_getxy
	���ܣ���ȡ�����������(x,y)��
	������ͨ��ָ��ķ�ʽ���뺯��	
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


	//ð��
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
