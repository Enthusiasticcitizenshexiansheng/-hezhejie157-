#ifndef __TIMER__H__
#define __TIMER__H__

#include "config.h"
#include "stm32f10x.h"


void Tim2Init(int prescaler,int period);


#define GENERAL_TIM3_Period 100
#define GENERAL_TIM3_Prescaler 72
void Time3Init(void);  //����Ϊ����Ƚ�ģʽ����������ͨ����pwm��


//��ʱ��4----------------

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



//��1---------------------------
#define Advance_TIM1_Period   (72000000/10000)  //����65535
#define Advance_TIM1_Prescaler 1
void Tim1Init(void);


//SPWM��---------------------------
#define SPWM_EN 0

#if SPWM_EN ==1
	#define FundamentalFrequency 50 //����Ƶ��
  #define SamplesPerHalfCycle   128 //������ռ�ձ� ����  
  
	#define SamplesAmplitude 4096.0   //spwmռ�ձ�
 
  #define CarrierFrequency  20000//spwm�ز�Ƶ��
  #define Tim1Period (72000000/(CarrierFrequency)) //�������ڲ�����65536
  #define Tim1Prescaler 1
  

 
  #define MaxDutyCycle 0.5
  
  #define Tim2Period (72000000/(SamplesPerHalfCycle*2*FundamentalFrequency))
  #define Tim2Prescaler 1


   void SpwmInit(void);
#endif
#endif


