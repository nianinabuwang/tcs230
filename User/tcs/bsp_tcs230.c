#include "bsp_tcs230.h"
#include "bsp_usart.h"
#include "motor.h"

u16 w_red=0;
u16 w_green=0;
u16 w_blue=0;	


void SysTick_Delay_Ms( __IO uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		// 当置1时，读取该位会清0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}



void TCS230_Init(void) 
	{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
//     NVIC_InitTypeDef NVIC_InitStructure;
    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置S0, S1, S2, S3引脚为输出
    GPIO_InitStructure.GPIO_Pin = TCS230_S2_PIN_Color|TCS230_S3_PIN_Color;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TCS230_GPIO, &GPIO_InitStructure);
    
    // 配置OUT引脚为输入捕获
    GPIO_InitStructure.GPIO_Pin = TCS230_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPD应该也可以
    GPIO_Init(TCS230_GPIO, &GPIO_InitStructure);
    
    // 使能TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 配置定时器，捕获的最小频率72M/(Per+1).现在采样为10khz
    TIM_TimeBaseStructure.TIM_Period = 0XFFFF ;
    TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // 配置输入捕获
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
		
		
//		    // 使能TIM3中断
//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM3, ENABLE);
    		/*选择触发源及从模式*/
		TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);					//触发源选择TI1FP1
		TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);					//从模式选择复位
		TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);



//    // 使能TIM3 CC1中断和更新中断
//    TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
//    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//				// 清除定时器中断标志位
//    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//    TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);													

}



//void TIM3_IRQHandler(void)
//{
//    // 检查输入捕获1中断标志位
//    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
//    {
//        // 清除中断标志位
//        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//				 // 读取捕获值
//     
//				
//							startTime = 10000/TIM_GetCapture1(TIM3);
//						 printf(" f=：%d\nHZ",startTime);		
//	}
//}

void Read_red(void)
{

	GPIO_ResetBits(TCS230_GPIO,TCS230_S2_PIN_Color);
	GPIO_ResetBits(TCS230_GPIO,TCS230_S3_PIN_Color);
}


void Read_green(void)
{
	GPIO_SetBits(TCS230_GPIO,TCS230_S2_PIN_Color);
	GPIO_SetBits(TCS230_GPIO,TCS230_S3_PIN_Color);
	
}

void Read_blue(void)
{
	GPIO_ResetBits(TCS230_GPIO,TCS230_S2_PIN_Color);
	GPIO_SetBits(TCS230_GPIO,TCS230_S3_PIN_Color);
}

void Read_none(void)
{
	GPIO_SetBits(TCS230_GPIO,TCS230_S2_PIN_Color);
	GPIO_SetBits(TCS230_GPIO,TCS230_S3_PIN_Color);
}

void white_recalibrate(void)
{
//	extern u16 w_red,w_blue,w_green;
	Read_red();
	SysTick_Delay_Ms(100);
	w_red=10000/TIM_GetCapture1(TIM3);
	Read_green();
	SysTick_Delay_Ms(100);
	w_green=10000/TIM_GetCapture1(TIM3);
	Read_blue();
	SysTick_Delay_Ms(100);
	w_blue=10000/TIM_GetCapture1(TIM3);
	printf("white recalibrate R,G,Bis(%d,%d,%d)\n\n\n",w_red,w_green,w_blue);
	SysTick_Delay_Ms(1000);
}

int rgb_in_range(u8 red, u8 green, u8 blue) {
    // 定义RGB值的范围
    u8 red_min = 100, red_max = 150;
    u8 green_min = 100, green_max = 150;
    u8 blue_min = 100, blue_max = 150;

    if ((red >= red_min && red <= red_max) && 
        (green >= green_min && green <= green_max) && 
        (blue >= blue_min && blue <= blue_max)) {
        return 1;
    }
    return 0;
}

void get_rgb(void)
{
	u8 red,green,blue;
	float z=1.00;
	Read_red();
	SysTick_Delay_Ms(200);
	red=(u8)(((10000/TIM_GetCapture1(TIM3)*z)/w_red)*255);
	
	
	Read_green();
	SysTick_Delay_Ms(200);
	green=(u8) (((10000/TIM_GetCapture1(TIM3)*z)/w_green)*255);

	
	Read_blue();
	SysTick_Delay_Ms(200);
	blue=(u8) (((10000/TIM_GetCapture1(TIM3)*z)/w_blue)*255);
	SysTick_Delay_Ms(100);
	
	printf("object R,G,B is(%d,%d,%d)\n\n\n",red,green,blue);
    
    if(rgb_in_range(red,green,blue))
    {
        printf("获取到指定颜色，停止电机\n");
        motor_Stop();
    }
    
}



