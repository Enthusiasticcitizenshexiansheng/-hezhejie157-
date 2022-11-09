#ifndef __DAC_H__
#define __DAC_H__

#include "config.h"
#include "stm32f10x.h"

void DacInit(void);
void DacSetVol(float vol);
//void DAC_SetChannellData(int,float vol);

#endif