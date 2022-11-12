#ifndef __INFLASH_H__
#define __INFLASH_H__

//#include"config.h"
#define uint8 unsigned char
#define uint16 unsigned short int
#define uint32 unsigned int
	

#define 	STM32_FLASH_SIZE  512
#define   STM32_FLASH_WREN   1


#if STM32_FLASH_SIZE <256  
  #define FLASH_PAGE_SIZE((uint16_t)0x400)
#else
 //  #define FLASH_PAGE_SIZE((uint16_t)0x800)
#endif


//从哪里读 读多少 读到哪里
int InFlashReadBytes(uint32 Addr,uint8 *pBuf,uint32 size);
int InFlashWriteBytes(uint32 Addr,uint8 *pBuf,uint32 size);


//FLASH与读写相关宏定义
#define CODE_SIZE((STM32_FLASH_SIZE/4)*0x400)  //1k*128
#define START_ADDR(0x08000000)
#define FLASH_USER_ADDR(START_ADDR+CODE_SIZE)  //存用户数据
#define stmInFlash_ReadBytes(Addr,Buff,Size)  InFlashReadBytes(FLASH_USER_ADDR+Addr,Buff,Size)
#define stmInFlash_WriteBytes(Addr,Buff,Size)  InFlashWriteBytes(FLASH_USER_ADDR+Addr,Buff,Size)



#endif


