#ifndef _APP_TFT_H
#define _APP_TFT_H
#include <stdio.h>


/**
 	- ads7843_RCC的SPI通信用GPIO口模拟。
 	- 在初始化时，程序会默认按照下面宏定义配置。
 	- 如果硬件连接有变化，请重新定义。
 	- ads7843_RCC所用用到引脚的时钟。
 	- ads7843_xx_port是xx引脚所在的端口。
 	- ads7843_xx为所在端口的个引脚。
  */
#define ads7843_rcc 	 RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO

#define ads7843_cs_port  GPIOB
#define ads7843_cs 		 GPIO_Pin_12

#define ads7843_sck_port GPIOB
#define ads7843_sck 	 GPIO_Pin_13

#define ads7843_mi_port  GPIOB
#define ads7843_mi 		 GPIO_Pin_14

#define ads7843_mo_port  GPIOB
#define ads7843_mo 		 GPIO_Pin_15

#define ads7843_by_port  GPIOG
#define ads7843_by 		 GPIO_Pin_8

#define ads7843_int_port GPIOG
#define ads7843_int 	 GPIO_Pin_7
#define ads7843_int_sor_prot  GPIO_PortSourceGPIOG 
#define ads7843_int_sor_pin   GPIO_PinSource7 
#define ads7843_int_exit_lin  EXTI_Line7 	 
#define ads7843_int_exit_irqn EXTI9_5_IRQn

#define ads7843_cs_1() 	GPIO_SetBits(ads7843_cs_port,ads7843_cs)
#define	ads7843_cs_0()	GPIO_ResetBits(ads7843_cs_port,ads7843_cs)

#define ads7843_mo_1() 	GPIO_SetBits(ads7843_mo_port,ads7843_mo)
#define	ads7843_mo_0()	GPIO_ResetBits(ads7843_mo_port,ads7843_mo)

#define ads7843_sck_1() GPIO_SetBits(ads7843_sck_port,ads7843_sck)
#define	ads7843_sck_0()	GPIO_ResetBits(ads7843_sck_port,ads7843_sck)

#define	ads7843_read_mi()	GPIO_ReadInputDataBit(ads7843_mi_port,ads7843_mi)
#define	ads7843_read_busy()	GPIO_ReadInputDataBit(ads7843_by_port,ads7843_by)


//**ssd1289初始化定义，全部在tft_init内部----------------------------


/** 16位色颜色定义*/
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#define CoppyB   	   0x0001



 
void tft_init(void);
void ssd1289_write_reg(uint16_t addr,uint16_t value);
void ssd1289_set_cursor(uint16_t x,uint16_t y);
void ssd1289_set_point(uint16_t x,uint16_t y,uint16_t clor);
void ssd1289_set_bkclor(uint16_t bkclor);
void ssd1289_putchar(uint16_t x,uint16_t y,uint16_t charclor,uint16_t bkclor,uint8_t ch);
void ssd1289_putchs(uint16_t x,uint16_t y,uint16_t chclor,uint16_t bkclor,uint8_t * ch);
void ssd1289_putchx(uint16_t x,uint16_t y,uint16_t chclor,uint16_t bkclor,uint8_t ch);
void ssd1289_square_full(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t clor);
void ssd1289_square_line(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t clor);
void ssd1289_draw_line(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t clor);
void ssd1289_putchxs(uint16_t x,uint16_t y,uint16_t chclor,uint16_t bkclor,uint8_t * ch);
void ssd1289_draw(uint16_t xs,uint8_t ys,const uint8_t * p);
void ssd1289_test(void);
void ads7843_send_cmd(uint8_t cmd);
uint16_t ads7843_read_data(void);
void ads7843_getxy(uint16_t *x,uint16_t *y);
#endif
