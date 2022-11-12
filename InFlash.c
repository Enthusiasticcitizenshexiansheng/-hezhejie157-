#include "config.h"
#include "InFlash.h"


int InFlashReadBytes(uint32_t Addr,uint8_t *pBuf,uint32_t size)
{
  unsigned int i;
	if(size ==0 )  return FALSE;
	for(i=0;i<size;i++)
	{
	  pBuf[i] = *(volatile unsigned char *)(Addr +i);
    
	}
	return 	TRUE;
}





static int InFlashWriteBytes(uint32_t Addr,uint8_t *pBuf,uint32_t size)
{
//不擦除 纯纯写
	  unsigned int iWrite =0;
    unsigned int WriteAddr =Addr;
	//指向数据的指针
	  unsigned int *pWriteData =(unsigned int *) pBuf;     
	
	
	   for(iWrite =0;iWrite<size/4;iWrite++)
	{
	FLASH_ProgramWord(WriteAddr, *pWriteData);
		WriteAddr+=4;
		pWriteData++;
	}
	
	if(size %4 ==0)
	{
	}
   else if(size%4==1)
	 {
	 FLASH_ProgramWord(WriteAddr, (*pWriteData+0xFFFFFF00));
	 }
   else if(size%4==2)
	 {
	 FLASH_ProgramWord(WriteAddr,  (*pWriteData+0xFFFF0000));
	 }
   else if(size%4==3)
	 {
	 FLASH_ProgramWord(WriteAddr,  (*pWriteData+0xFF000000));
	 }
	  


}

int InFlashWriteBytes(uint32_t Addr,uint8_t *pBuf,uint32_t size)
{

}


















