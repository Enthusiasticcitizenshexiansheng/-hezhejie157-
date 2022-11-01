#ifndef __ADC_H__
#define __ADC_H__




#include "config.h"
#include "stm32f10x.h"


#define ADC_EN 1


#if ADC_EN ==1

#define NbrOfChannel 4
extern __IO uint16 ADCConvertedValue[NbrOfChannel];

void AdcInit(void);
#endif



#endif

