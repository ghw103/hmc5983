#ifndef __SRAM_H
#define __SRAM_H															    
#include "stm32f4xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
#define Bank1_SRAM3_ADDR    ((uint32_t)(0x68000000))								  
//////////////////////////////////////////////////////////////////////////////////
typedef __IO uint32_t  vuint32_t;
typedef __IO uint16_t vuint16_t;
typedef __IO uint8_t  vuint8_t;
											  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumHalfwordToRead);
void fsmc_sram_test(uint16_t x,uint16_t y);

//void fsmc_sram_test_write(uint32_t addr,uint8_t data);
//uint8_t fsmc_sram_test_read(uint32_t addr);


#endif

