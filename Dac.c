#include"Dac.h"



void  DacGpioInit(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//收指针

}


void	 DacModeInit(void)
{

  DAC_InitTypeDef DAC_InitType;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
  DAC_InitType.DAC_Trigger=DAC_Trigger_Software;
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None ;
  DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bits11_0  ;
  DAC_InitType.DAC_OutputBuffer =DAC_OutputBuffer_Enable ;
  
	DAC_Init(DAC_Channel_1,&DAC_InitType);
  DAC_Cmd(DAC_Channel_1,ENABLE); 
 }


void DacInit(void)
{
	
	 DacGpioInit();
	 DacModeInit();
}



//输出DA值
void DacSetVol(float vol)
{
  vol =(vol/3.3)*4095;
  //DAC_SetChannellData(DAC_Align_12b_R,vol);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}

