#include"system.h"

void CpuInit(void)
{
//CPU�������
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ��ڶ����ж����ȼ���2λ�����ȼ�
	
	
}

void SysInit(void)
{
   CpuInit();
	//���������ĳ�ʼ��
	void Tim3Init(void);
	
	void Tim4Init(void);
	SysTickInit();//ϵͳ�δ�ʱ����ʼ��
	LedGpioInit();//led��ʼ��
	BeepGpioInit();//Beep��������ʼ��
	UsartInit();//���ڳ�ʼ��
//Tim2Init(7200,10000);
	Tim3Init();
	Tim4Init();//���벶��ģʽpb6
//	Tim1Init(); //����Ƚ�ģʽ  CH1 CH1N
  ExtiInit();//P11�ⲿ�жϳ�ʼ��
#if SPWM_EN ==1
	SpwmInit();
#else
//	Tim2Init(7200,10000);
//	Tim1Init(); //����Ƚ�ģʽ  CH1 CH1N
#endif
#if ADC_EN==1
	AdcInit();
#endif
	TaskInit();//�����ʼ��
}








//p5������ģ������


void ATask(void);
void BTask(void);
void CTask(void);
void Usart1ControlBeepTask(void);
void AdcTask(void); //���� ADC������


TaskStruct  Task[] ={
	
	{ATask,200},
  {BTask,300},
	{CTask,2000},
  {Usart1ControlBeepTask,20},
  { AdcTask,500}
	//{��������������������ڣ���λms��}
	
};

void TaskInit(void)
{	//�����ʼ��

	
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


	//�����������
	if(++cnt%2==0)
	{GPIO_SetBits(GPIOA, GPIO_Pin_0 );
		cnt=0;}
	else
 GPIO_ResetBits(GPIOA,GPIO_Pin_0 );
	
	

}


void BTask(void)
{			static uint8 cnt;

	//�����������
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

	//�����������
	
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

	//�����������
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



