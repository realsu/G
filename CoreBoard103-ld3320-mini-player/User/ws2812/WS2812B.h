#ifndef __WS2812B_H
#define	__WS2812B_H

#include "stm32f10x.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "delay.h"	

//#define WS2812_IN_PIN	PA0

void Timer2_init(void);
void WS2812_send(uint8_t (*color)[3], uint16_t len);

#endif /* __LED_H */
