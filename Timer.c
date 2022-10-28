#include "Timer.h"


 void Tim2ModeInit(int prescaler,int period)
 {
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	 
	 TIM_DeInit(TIM2);
	 TIM_InternalClockConfig(TIM2);
	 TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(prescaler-1);
	 TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	 TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	 TIM_TimeBaseStructure.TIM_Period= (period<2)?1:period -1;
	 
	 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
   TIM_ARRPreloadConfig(TIM2,DISABLE);
 }

 
 void  Tim2NvicInit(void)
 {  
	 NVIC_InitTypeDef NVIC_InitStructure;
//����TIM�ж�	
	 NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;                    
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;      
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
   NVIC_Init(&NVIC_InitStructure);
 }
 
 
void Tim2Init(int prescaler,int period)
{
  Tim2ModeInit(prescaler,period);
	Tim2NvicInit();//�ж����ȼ�
	 

  TIM_ClearFlag(TIM2,TIM_FLAG_Update);//�������жϱ�־
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//����TIM2�ж�
	TIM_Cmd(TIM2,ENABLE);
}






void TIM2_IRQHandler(void)
{
if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
{

	UsartSendString(USART2,"TIMER2 update Interrupt occur!\n");
	//�û�����
	TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
}

}
	















//p12.2���pwm--------------------------------------
static void Tim3GpioInit(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	
	//����Ƚ�ͨ��1 GPIO��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ָ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ָ��
}


static void Tim3ModeInit(void)
{//��ʹ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//����װ�ؼĴ�����ֵ�����ڸ���
	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM3_Period-1;
	//����cnt��������ʱ�� = FCK_INT/(PSC+1)
  TIM_TimeBaseStructure.TIM_Prescaler =GENERAL_TIM3_Prescaler-1;
  //ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	//���ϼ���
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //�ظ���������ֵ��û�õ�����
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  //��ʼ����ʱ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
/*---------------����ȽϽṹ��仯-------------------*/
	uint16_t CCR1_Val=50;
 	uint16_t CCR2_Val=30;
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	//����pwmΪģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 //ʹ�ܼӵ�ƽ��������
  TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;
	
	
	
  //����Ƚ�ͨ��1
	TIM_OCInitStructure.TIM_Pulse =CCR1_Val;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	  //����Ƚ�ͨ��2
	TIM_OCInitStructure.TIM_Pulse =CCR2_Val;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	//ʹ�ܼ�����
	 TIM_Cmd(TIM3,ENABLE);
}	




void Tim3Init(void)
{
  Tim3GpioInit();
	Tim3ModeInit();
}



//-----------------

 void   Tim4GpioInit(void);
 void 	Tim4ModeInit(void);
 void 	Tim4NvicInit(void);
 void   Time4Init(void);

void Tim4Init(void)
{
  Tim4GpioInit();
	Tim4ModeInit();
	Tim4NvicInit();
	
	
	//����������ж�
		  TIM_ClearFlag(TIM4,TIM_FLAG_Update|TIM_IT_CC1);//�������жϱ�־
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);//����TIM2�ж�
	TIM_Cmd(TIM4,ENABLE);

	
}


void  Tim4GpioInit(void)
{ GPIO_InitTypeDef GPIO_InitStructure;
	
	//Tim4���벶��ͨ��1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ָ��
  
	//GPIO_ResetBits(GPIOB, GPIO_Pin_6);
}


void 	Tim4ModeInit(void)
{    TIM_ICInitTypeDef TIM_ICInitStruct;
  
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	 
	 TIM_DeInit(TIM4);
	 TIM_InternalClockConfig(TIM4);
	//��Ƶ�� 72  1mHz
	 TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(GENERAL_TIM4_Prescaler);
	 TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	//���ϼ���
	 TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	//���������С
	TIM_TimeBaseStructure.TIM_Period= GENERAL_TIM4_Period-1 ;
	 //����Ӧ��
	 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
   TIM_ARRPreloadConfig(TIM4,DISABLE);
	
	
	
	
	//����ͨ��һ
	TIM_ICInitStruct.TIM_Channel=  TIM_Channel_1;
//ֱͨ  
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
  //���Ĵ������زż���
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV4;
	//�˲� ����
  TIM_ICInitStruct.TIM_ICFilter=0x00;
	//������
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	
	//���벶��
	TIM_ICInit( TIM4, & TIM_ICInitStruct);
}

 


void 	Tim4NvicInit(void)	
{ 
	 NVIC_InitTypeDef NVIC_InitStructure;
//����TIM�ж�	
	 NVIC_InitStructure.NVIC_IRQChannel= TIM4_IRQn;                    
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;      
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
   NVIC_Init(&NVIC_InitStructure);


}




//p13-3
WaveCapture waveCapture={RESET,0,0,0,0};



//�жϻص�����
void TIM4_IRQHandler(void)
{ 
	if(waveCapture.ucFinishFlag==0)
	{
	                 //�����ж�
  if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
{

	//�û�����
	if(	waveCapture.Egde == SET)
	waveCapture.usUpdateCnt ++;
	//
	TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
}


                 //����Ƚ�
  if(TIM_GetITStatus(TIM4,TIM_IT_CC1)!=RESET)
{
	//�û�����
	
	if(waveCapture.Egde == RESET)  //��һ�β���
	{
	
	waveCapture.usCaptureRisingVal=0;//���벶��ֵ
	waveCapture.usUpdateCnt  =0;//����������
	waveCapture.Egde = SET;
		//����
	TIM_SetCounter(TIM4,0);
	}
 else 
 {
  waveCapture.usCaptureRisingVal=TIM_GetCapture1(TIM4);
	 //Ƶ��
	waveCapture.ulFrequency = 1000000/(waveCapture.usUpdateCnt * 65536 +waveCapture.usCaptureRisingVal);
	waveCapture.usCaptureRisingVal=0;//���벶��ֵ
	waveCapture.usUpdateCnt  =0;//����������
	waveCapture.Egde = RESET;
	 //������־
	waveCapture.ucFinishFlag =1;
 }

	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
}
	 
}
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_Update);
}

