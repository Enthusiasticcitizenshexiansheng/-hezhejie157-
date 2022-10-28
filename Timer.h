#ifndef __TIMER__H__
#define __TIMER__H__

#include "config.h"
#include "stm32f10x.h"


void Tim2Init(int prescaler,int period);


#define GENERAL_TIM3_Period 100
#define GENERAL_TIM3_Prescaler 72
void Time3Init(void);  //����Ϊ����Ƚ�ģʽ����������ͨ����pwm��


//------------------

typedef struct 
{
  FlagStatus Egde;//Egde = SET ��ʾ��ǰ���ڸߵ�ƽ
	uint8 ucFinishFlag;//����������
	uint16 usCaptureRisingVal;//���벶��ֵ
	uint16 usUpdateCnt;//����������
	uint32 ulFrequency;//���벨��Ƶ��
	
}WaveCapture;


extern WaveCapture waveCapture ;

#define GENERAL_TIM4_Period   0xffff
#define GENERAL_TIM4_Prescaler 72
void Time4Init(void);  //����Ϊ���벶��ģʽ������һ��ͨ����pwm��
#endif

