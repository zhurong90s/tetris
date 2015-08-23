/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdlib.h>
extern uint8_t Lift,Right,Up,Down;
extern uint16_t CurX,CurY;
extern uint8_t GlNu,GlNextNu;
extern struct SHAPE Boxs[MAX_BOX];
extern  uint8_t flage,gameover,point;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */


void SysTick_Handler(void)
{
	static uint8_t	Scanb = 0,Scanb2 =0,sec=0;	

	   Scanb++;
	   Scanb2++;
	   sec++;
	   if(Scanb >9)	   
	   {	Scanb = 0;
	   		if(Lift == 1)
			{
				if(mov_able(CurX ,CurY-20,GlNu) == able)
				{
					eraser_box(CurX,CurY,GlNu);
					CurY -= 20;
					show_box(CurX,CurY,GlNu); 	
				}	
			}else if(Right == 1)
			{
				if(mov_able(CurX ,CurY+20,GlNu) == able)
				{
					eraser_box(CurX,CurY,GlNu);
					CurY += 20;
					show_box(CurX,CurY,GlNu);
				}	
			}else if(Down == 1)
			{
				if(mov_able(CurX+20,CurY,GlNu) == able)
				{
					eraser_box(CurX,CurY,GlNu);
					CurX += 20;
					show_box(CurX,CurY,GlNu);
				}	
			} 
			
		}
		if(Scanb2 > 21)
		{ 
			Scanb2 = 0;
			if(Up == 1)
			{
				if(mov_able(CurX,CurY,Boxs[GlNu].next) == able)
				{
					eraser_box(CurX,CurY,GlNu);
					GlNu = Boxs[GlNu].next;
					show_box(CurX,CurY,GlNu);
				}
	
			}
			   
	    }

		if(sec > 99)
		{
			sec = 0;
			//判断是否可以继续向下移动
			if(mov_able(CurX+20,CurY,GlNu) == able)
			{
				eraser_box(CurX,CurY,GlNu);
				CurX += 20;
				show_box(CurX,CurY,GlNu);
			}else
			{
				point += updata_board(CurX,CurY,GlNu)*5;
				if(point < 240)
				{
					show_point(point);
					GlNu = GlNextNu;
					CurX = 5;
					CurY = 65;
					/*<查看是否可以产生新的方框>-------------------------------------------------------*/
					if(mov_able(CurX,CurY,GlNu) == able)
						show_box(CurX,CurY,GlNu);
					else
						{
							gameover = 1;
							SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
						}		
				}else
				{
					SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
					ssd1289_set_bkclor(Black);
					ssd1289_putchs(5,5,Red,Black,"You break my Point cup, wawa,So I shutdown the machine !gaga~~");
					while(1);
						
				}	
			}

		}

		SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
	
void EXTI9_5_IRQHandler (void)
{
	static uint16_t x,y;
	uint32_t i;
	if( EXTI_GetITStatus (EXTI_Line7) == SET )
	{
		ads7843_getxy(&x,&y);
		if((290<x)&&(x<315)&&(5<y)&&(y<45))
		{
			//第一个
			ssd1289_square_full(290,5,315,45,BUTTON_DOWN);
			ssd1289_putchs(295,10,Black,CoppyB,"left");
			Lift = 1;
			//按下了~~~~~~
			while(GPIO_ReadInputDataBit(ads7843_int_port,ads7843_int) == RESET)
			GlNextNu = rand()%19;
			i=60;
			while(i != 0) i--;
			while(GPIO_ReadInputDataBit(ads7843_int_port,ads7843_int) == RESET)
			GlNextNu = rand()%19;
			ssd1289_square_full(290,5,315,45,BUTTON_UP);
			ssd1289_putchs(295,10,Black,CoppyB,"Left");
			Lift = 0;
		}else if((290<x)&&(x<315)&&(68<y)&&(y<108))
		{
			//第二个
			ssd1289_square_full(290,68,315,108,BUTTON_DOWN);
			ssd1289_putchs(295,68,Black,CoppyB,"right");
			Right = 1;
			while(GPIO_ReadInputDataBit(ads7843_int_port,ads7843_int) == RESET)
			GlNextNu = rand()%19;
			i=60;
			while(i != 0) i--;
			while(GPIO_ReadInputDataBit(ads7843_int_port,ads7843_int) == RESET)
			GlNextNu = rand()%19;
			ssd1289_square_full(290,68,315,108,BUTTON_UP);
			ssd1289_putchs(295,68,Black,CoppyB,"Ringt");
			Right = 0;	
		}else if((290<x)&&(x<315)&&(131<y)&&(y<171))
		{
			//第三个
			ssd1289_square_full(290,131,315,171,BUTTON_DOWN);
			ssd1289_putchs(295,136,Black,CoppyB,"down");
			Down = 1;
			while(GPIO_ReadInputDataBit(ads7843_int_port,ads7843_int) == RESET)
			GlNextNu = rand()%19;
			i=60;
			while(i != 0) i--;
			while(GPIO_ReadInputDataBit(ads7843_int_port,ads7843_int) == RESET)
			GlNextNu = rand()%19;
			ssd1289_square_full(290,131,315,171,BUTTON_UP);
			ssd1289_putchs(295,136,Black,CoppyB,"Down");
			Down = 0;
		}else if((290<x)&&(x<315)&&(194<y)&&(y<234))
		{
			//第四个
			ssd1289_square_full(290,194,315,234,BUTTON_DOWN);
			ssd1289_putchs(295,204,Black,CoppyB,"up");
			Up = 1;
			while(GPIO_ReadInputDataBit(ads7843_int_port,ads7843_int) == RESET)
			GlNextNu = rand()%19;
			i=60;
			while(i != 0) i--;
			while(GPIO_ReadInputDataBit(ads7843_int_port,ads7843_int) == RESET)
			GlNextNu = rand()%19;
			ssd1289_square_full(290,194,315,234,BUTTON_UP);
			ssd1289_putchs(295,204,Black,CoppyB,"Up");
			Up = 0;	
		}else if((x<100)&&(y>100))
		{
			flage = 1;	
		}

		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}
/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
