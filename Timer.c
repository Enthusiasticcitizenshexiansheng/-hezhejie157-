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

	UsartSendString(USART2,"TIMER2 update Interrupt occur!\n");
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

 void  Tim4GpioInit(void);
 void 	Tim4ModeInit(void);
 void 	Tim4NvicInit(void);


void Time4Init(void)
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
  TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
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


//中断回调函数
void TIM4_IRQHandler(void)
{
if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
{

	//用户代码
	TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
}


if(TIM_GetITStatus(TIM4,TIM_IT_CC1)!=RESET)
{
	//用户代码
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
}
	  //TIM_ClearITPendingBit(TIM4,TIM_IT_CC1|TIM_IT_Update);
}


