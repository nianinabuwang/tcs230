#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

void motor_Init(void);
void motor_Start(void);
void motor_Stop(void);
void motor_Enable(void);
void motor_Disable(void);



extern volatile uint8_t motor_running; 

#endif /* __MOTOR_H */
