#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include"config.h"
//#include"stm32f10x.h"


typedef struct
{
	void (*pTask)(void); //������ָ��
	uint32 TaskPeriod; //������������
	
}TaskStruct;

void SysInit(void);//ϵͳ��ʼ��

void TaskInit(void);//�����ʼ�� ������������������
void TaskRun(void);//��������


#endif


