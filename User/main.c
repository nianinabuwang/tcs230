/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_tcs230.h"
#include "motor.h"
#include "key.h"
#include <stdio.h>

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
  
  int main(void)
{	
   
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	
	/* 发送一个字符串 */
	Usart_SendString( DEBUG_USARTx,"这是一个串口中断接收回显实验\n");
	TCS230_Init();
	white_recalibrate();
    
    key_Init();
	motor_Init();
    
   
  while(1)
	{	
		get_rgb();
   
		SysTick_Delay_Ms(500);
        
   
	}
    
}
/*********************************************END OF FILE**********************/
