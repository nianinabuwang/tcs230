#include "motor.h"
//#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "bsp_tcs230.h"

volatile uint8_t motor_running = 0;

void motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef TIM_OCInitStructu。re;

    // 启用TIM3和GPIOA时钟
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置GPIOA的引脚为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; // 使用PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    motor_Stop();
    
    
    // 配置TIM3为PWM输出模式
//    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 设置定时器的周期，决定了脉冲的频率
//    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 设置预分频器，决定了定时器的时钟频率
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM模式1
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = 500; // 设置初始占空比，决定了脉冲的宽度
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

//    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
//    
//    TIM_Cmd(TIM3, DISABLE);
}

void motor_Start(void)
{ 
//    TIM_Cmd(TIM3, ENABLE);
    motor_running = 1;
    while(motor_running){
        GPIO_SetBits(GPIOA,GPIO_Pin_7);
        SysTick_Delay_Ms(1);
        GPIO_ResetBits(GPIOA,GPIO_Pin_7);
        SysTick_Delay_Ms(1);
           
    }
}

void motor_Stop(void)
{
//    TIM_Cmd(TIM3, DISABLE);
    motor_running = 0;
    GPIO_SetBits(GPIOA,GPIO_Pin_7);
}



