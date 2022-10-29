#ifndef __TIMER__H__
#define __TIMER__H__

#include "config.h"
#include "stm32f10x.h"


void Tim2Init(int prescaler,int period);


#define GENERAL_TIM3_Period 100
#define GENERAL_TIM3_Prescaler 72
void Time3Init(void);  //配置为输出比较模式，产生两个通道的pwm波


//定时器4----------------

typedef struct 
{
  FlagStatus Egde;//Egde = SET 表示当前处于高电平
	uint8 ucFinishFlag;//捕获结束标记
	uint16 usCaptureRisingVal;//输入捕获值
	uint16 usUpdateCnt;//计数溢出标记
	uint32 ulFrequency;//输入波形频率
	
}WaveCapture;


extern WaveCapture waveCapture ;

#define GENERAL_TIM4_Period   0xffff
#define GENERAL_TIM4_Prescaler 72
void Time4Init(void);  //配置为输入捕获模式，产生一个通道的pwm波



//定1---------------------------
#define Advance_TIM1_Period   (72000000/10000)  //不超65535
#define Advance_TIM1_Prescaler 1
void Tim1Init(void);

#endif

