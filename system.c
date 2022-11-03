#include"system.h"

void CpuInit(void)
{
//CPU相关设置
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择第二组中断优先级，2位主优先级
	
	
}

void SysInit(void)
{
   CpuInit();
	//外设驱动的初始化
	void Tim3Init(void);
	
	void Tim4Init(void);
	SysTickInit();//系统滴答定时器初始化
	LedGpioInit();//led初始化
	BeepGpioInit();//Beep蜂鸣器初始化
	UsartInit();//串口初始化
//Tim2Init(7200,10000);
	Tim3Init();
	Tim4Init();//输入捕获模式pb6
//	Tim1Init(); //输出比较模式  CH1 CH1N
  ExtiInit();//P11外部中断初始化
#if SPWM_EN ==1
	SpwmInit();
#else
//	Tim2Init(7200,10000);
//	Tim1Init(); //输出比较模式  CH1 CH1N
#endif
#if ADC_EN==1
	AdcInit();
#endif
	TaskInit();//任务初始化
}








//p5多任务模拟运行


void ATask(void);
void BTask(void);
void CTask(void);
void Usart1ControlBeepTask(void);
void AdcTask(void); //声明 ADC任务函数


TaskStruct  Task[] ={
	
	{ATask,200},
  {BTask,300},
	{CTask,2000},
  {Usart1ControlBeepTask,20},
  { AdcTask,500}
	//{任务函数名，任务调用周期（单位ms）}
	
};

void TaskInit(void)
{	//任务初始化

	
      uint8 NTask = 0;
	    for(NTask = 0;NTask< sizeof(Task)/sizeof(Task[0]);NTask++ )
	  TaskTimer[NTask] = Task[NTask].TaskPeriod;
	
}

void TaskRun(void)
{ 
	   uint8 NTask = 0;
	   for(NTask = 0;NTask< sizeof(Task)/sizeof(Task[0]);NTask++ )
	   if(	TaskTimer[NTask] == 0)
		 {			 
     	TaskTimer[NTask] = Task[NTask].TaskPeriod;
			 (Task[NTask].pTask)();
		 }
	 


}

void ATask(void)
{
	static uint8 cnt;


	//补充任务代码
	if(++cnt%2==0)
	{GPIO_SetBits(GPIOA, GPIO_Pin_0 );
		cnt=0;}
	else
 GPIO_ResetBits(GPIOA,GPIO_Pin_0 );
	
	

}


void BTask(void)
{			static uint8 cnt;

	//补充任务代码
		if(++cnt%2==0)
		{
 GPIO_SetBits(GPIOA,GPIO_Pin_1);
	cnt=0;
		}
		else
 GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	
}



void CTask(void)
{			

	//补充任务代码
	
	printf("System total Run Time is %f s\n",GetRunTime()/1000.0);
  UsartSendString(USART1,"Ctask is Running.\n---------------------\n");
	UsartSendString(USART2,"chuanko2 zai pao.\n---------------------\n");


  TIM_SetCompare1(TIM3,(TIM_GetCapture1(TIM3)+10)%100);
  TIM_SetCompare2(TIM3,(TIM_GetCapture1(TIM3)+10)%100);

	if(waveCapture.ucFinishFlag==1)
 	{
	printf("Frequency = %d Hz\n",waveCapture.ulFrequency);
	waveCapture.ucFinishFlag = 0;
	}else
	{
	printf("wave is not exit or cature is incomplete");
	}
}




void 	Usart1ControlBeepTask()
{

	//补充任务代码
	  Usart1_RecProcess();
}



void AdcTask(void)
{
#if ADC_EN ==1
	printf("\r\n ADC CH10 (PC0) value =%2fV \r\n",(float)ADCConvertedValue[0]/4096 * 3.3);
	printf("\r\n ADC CH11 (PC1) value =%2fV \r\n",(float)ADCConvertedValue[1]/4096 * 3.3);
	printf("\r\n ADC CH12 (PC2) value =%2fV \r\n",(float)ADCConvertedValue[2]/4096 * 3.3);
	printf("\r\n ADC CH13 (PC3) value =%2fV \r\n",(float)ADCConvertedValue[3]/4096 * 3.3);
#endif

}



