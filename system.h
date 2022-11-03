#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include"config.h"
//#include"stm32f10x.h"


typedef struct
{
	void (*pTask)(void); //任务函数指针
	uint32 TaskPeriod; //任务运行周期
	
}TaskStruct;

void SysInit(void);//系统初始化

void TaskInit(void);//任务初始化 、包含运行周期设置
void TaskRun(void);//任务运行


#endif


