#include "Timer.h"

#if SPWM_EN ==1
static uint8 bIPostiveHalf = 0;
static uint16 SineTablePos =0;
static uint16 DutyCycleSineWaveTable[SamplesPerHalfCycle] =
{
0x0000,0x0064,0x00C8,0x012D,0x0191,0x01F5,0x0259,0x02BC,0x031F,0x0381,0x03E3,0x0444,0x04A5,0x0504,0x0563,0x05C2,
0x061F,0x067B,0x06D7,0x0731,0x078A,0x07E2,0x0839,0x088F,0x08E3,0x0936,0x0987,0x09D7,0x0A26,0x0A73,0x0ABE,0x0B08,
0x0B50,0x0B96,0x0BDA,0x0C1D,0x0C5E,0x0C9D,0x0CD9,0x0D14,0x0D4D,0x0D84,0x0DB9,0x0DEB,0x0E1C,0x0E4A,0x0E76,0x0EA0,
0x0EC8,0x0EED,0x0F10,0x0F31,0x0F4F,0x0F6B,0x0F85,0x0F9C,0x0FB1,0x0FC3,0x0FD3,0x0FE1,0x0FEC,0x0FF4,0x0FFB,0x0FFE,
0x1000,0x0FFE,0x0FFB,0x0FF4,0x0FEC,0x0FE1,0x0FD3,0x0FC3,0x0FB1,0x0F9C,0x0F85,0x0F6B,0x0F4F,0x0F31,0x0F10,0x0EED,
0x0EC8,0x0EA0,0x0E76,0x0E4A,0x0E1C,0x0DEB,0x0DB9,0x0D84,0x0D4D,0x0D14,0x0CD9,0x0C9D,0x0C5E,0x0C1D,0x0BDA,0x0B96,
0x0B50,0x0B08,0x0ABE,0x0A73,0x0A26,0x09D7,0x0987,0x0936,0x08E3,0x088F,0x0839,0x07E2,0x078A,0x0731,0x06D7,0x067B,
0x061F,0x05C2,0x0563,0x0504,0x04A5,0x0444,0x03E3,0x0381,0x031F,0x02BC,0x0259,0x01F5,0x0191,0x012D,0x00C8,0x0064

};

#endif


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
//配置TIM中断	
	 NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;                    
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;      
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
   NVIC_Init(&NVIC_InitStructure);
 }
 
 
void Tim2Init(int prescaler,int period)
{
  Tim2ModeInit(prescaler,period);
	Tim2NvicInit();//中断优先级
	 

  TIM_ClearFlag(TIM2,TIM_FLAG_Update);//清除溢出中断标志
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//开启TIM2中断
	TIM_Cmd(TIM2,ENABLE);
}






void TIM2_IRQHandler(void)
{
if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
{
#if SPWM_EN ==1
	  if(bIPostiveHalf == 0)
		{
		  TIM_SetCompare1(TIM1,((DutyCycleSineWaveTable[SineTablePos]/SamplesAmplitude)*Tim1Period)/MaxDutyCycle);
      TIM_SetCompare2(TIM1,0);
			SineTablePos++;
			if(SineTablePos==128)
			{
			SineTablePos = 0 ;
			bIPostiveHalf = 1;
			}
			
		}
		else
		{
	  TIM_SetCompare1(TIM1,0);
      TIM_SetCompare2(TIM1,((DutyCycleSineWaveTable[SineTablePos]/4096.0)*Tim1Period)/2.0);
			SineTablePos++;
			if(SineTablePos==128)
			{
			SineTablePos = 0 ;
			bIPostiveHalf = 0;
			}
		
		}
		
	#else
	UsartSendString(USART2,"TIMER2 update Interrupt occur!\n");
	#endif
	//用户代码
	
	TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
}

}
	















//p12.2输出pwm--------------------------------------
static void Tim3GpioInit(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	
	//输出比较通道1 GPIO初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//收指针
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//收指针
}


static void Tim3ModeInit(void)
{//开使能
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//重新装载寄存器的值，用于更新
	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM3_Period-1;
	//驱动cnt计数器的时钟 = FCK_INT/(PSC+1)
  TIM_TimeBaseStructure.TIM_Prescaler =GENERAL_TIM3_Prescaler-1;
  //时钟分频因子
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	//向上计数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //重复计数器的值，没用到不管
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  //初始化定时器
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
/*---------------输出比较结构体变化-------------------*/
	uint16_t CCR1_Val=50;
 	uint16_t CCR2_Val=30;
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	//配置pwm为模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 //使能加电平极性配置
  TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;
	
	
	
  //输出比较通道1
	TIM_OCInitStructure.TIM_Pulse =CCR1_Val;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	  //输出比较通道2
	TIM_OCInitStructure.TIM_Pulse =CCR2_Val;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	//使能计数器
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
	
	
	//清除后允许中断
		  TIM_ClearFlag(TIM4,TIM_FLAG_Update|TIM_IT_CC1);//清除溢出中断标志
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);//开启TIM2中断
	TIM_Cmd(TIM4,ENABLE);

	
}


void  Tim4GpioInit(void)
{ GPIO_InitTypeDef GPIO_InitStructure;
	
	//Tim4输入捕获通道1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//收指针
  
	//GPIO_ResetBits(GPIOB, GPIO_Pin_6);
}


void 	Tim4ModeInit(void)
{    TIM_ICInitTypeDef TIM_ICInitStruct;
  
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	 
	 TIM_DeInit(TIM4);
	 TIM_InternalClockConfig(TIM4);
	//分频器 72  1mHz
	 TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(GENERAL_TIM4_Prescaler);
	 TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	//向上计数
	 TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	//计数溢出大小
	TIM_TimeBaseStructure.TIM_Period= GENERAL_TIM4_Period-1 ;
	 //配置应用
	 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
   TIM_ARRPreloadConfig(TIM4,DISABLE);
	
	
	
	
	//配置通道一
	TIM_ICInitStruct.TIM_Channel=  TIM_Channel_1;
//直通  
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
  //有四次上升沿才继续
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV4;
	//滤波 几次
  TIM_ICInitStruct.TIM_ICFilter=0x00;
	//上升沿
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	
	//输入捕获
	TIM_ICInit( TIM4, & TIM_ICInitStruct);
}

 


void 	Tim4NvicInit(void)	
{ 
	 NVIC_InitTypeDef NVIC_InitStructure;
//配置TIM中断	
	 NVIC_InitStructure.NVIC_IRQChannel= TIM4_IRQn;                    
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;      
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
   NVIC_Init(&NVIC_InitStructure);


}




//p13-3
WaveCapture waveCapture={RESET,0,0,0,0};



//中断回调函数
void TIM4_IRQHandler(void)
{ 
	if(waveCapture.ucFinishFlag==0)
	{
	                 //更新中断
  if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
{

	//用户代码
	if(	waveCapture.Egde == SET)
	waveCapture.usUpdateCnt ++;
	//
	TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
}


                 //捕获比较
  if(TIM_GetITStatus(TIM4,TIM_IT_CC1)!=RESET)
{
	//用户代码
	
	if(waveCapture.Egde == RESET)  //第一次捕获
	{
	
	waveCapture.usCaptureRisingVal=0;//输入捕获值
	waveCapture.usUpdateCnt  =0;//计数溢出标记
	waveCapture.Egde = SET;
		//清零
	TIM_SetCounter(TIM4,0);
	}
 else 
 {
  waveCapture.usCaptureRisingVal=TIM_GetCapture1(TIM4);
	 //频率
	waveCapture.ulFrequency = 1000000/(waveCapture.usUpdateCnt * 65536 +waveCapture.usCaptureRisingVal);
	waveCapture.usCaptureRisingVal=0;//输入捕获值
	waveCapture.usUpdateCnt  =0;//计数溢出标记
	waveCapture.Egde = RESET;
	 //结束标志
	waveCapture.ucFinishFlag =1;
 }

	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
}
	 
}
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_Update);
}



//定1------------------------

 void   Tim1GpioInit(void);
 void 	Tim1ModeInit(void);

void Tim1Init(void)
{
  Tim1GpioInit();
	Tim1ModeInit();
}


 void   Tim1GpioInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);	 
		 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;//TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  
	GPIO_ResetBits(GPIOE, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);

}


 void 	Tim1ModeInit(void)
 {
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;
	 
	 
	 //开时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
 	 TIM_DeInit(TIM1); 
	 /* TIM1 Peripheral Configuration ----------------------------------------*/
  /* Time Base configuration */
#if  SPWM_EN ==1
  TIM_TimeBaseStructure.TIM_Prescaler =(uint16)(Tim1Prescaler-1);
  TIM_TimeBaseStructure.TIM_Period = (uint16)(Tim1Period-1);
#else
	 TIM_TimeBaseStructure.TIM_Prescaler =(uint16)( Advance_TIM1_Prescaler-1);
  TIM_TimeBaseStructure.TIM_Period = (uint16)(Advance_TIM1_Period-1);
	 
#endif
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Advance_TIM1_Period*0.3;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);//ccr
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
/* Automatic Output enable, Break, dead time and lock configuration*/
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
  TIM_BDTRInitStructure.TIM_DeadTime = 72;  //1/72m  *10个单位时间
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);






   TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Enable);
   TIM_OC2PreloadConfig( TIM1, TIM_OCPreload_Enable);
  
	TIM_ARRPreloadConfig(TIM1,ENABLE);  //arr值更改要等一个周期
  
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);

 }




 //spwm----------------------------------------------------------

#if SPWM_EN ==1


   void SpwmInit(void)
	 {
	   Tim2Init(Tim2Prescaler,Tim2Period);
		 Tim1Init();
	 }
#endif
 
 
 

