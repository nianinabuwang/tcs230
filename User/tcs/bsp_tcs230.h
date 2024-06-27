#ifndef __tcs_H
#define	__tcs_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
// 定义引脚,s0,s1已经接好高低电平，以20%频率输出
//#define TCS230_S0_PIN_Speed   GPIO_Pin_0
//#define TCS230_S1_PIN_Speed   GPIO_Pin_1
#define TCS230_S2_PIN_Color   GPIO_Pin_2
#define TCS230_S3_PIN_Color   GPIO_Pin_3
#define TCS230_OUT_PIN  GPIO_Pin_6//TIM3_CH1
#define TCS230_GPIO     GPIOA

//struct tcs230
//{
//	u16 w_red;
//	u16 w_green;
//	u16 w_blue;
//	u16 red;
//	u16 green;
//	u16 blue;
//	
//};



void TCS230_Init(void);
void Read_red(void);
void Read_green(void);
void Read_blue(void);
void Read_none(void);
void white_recalibrate(void);
void get_rgb(void);
void SysTick_Delay_Ms( __IO uint32_t ms);
#endif

