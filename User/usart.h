#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

void USART_Config(void);
int fputc(int ch, FILE *f);

#endif /* __USART_H */
