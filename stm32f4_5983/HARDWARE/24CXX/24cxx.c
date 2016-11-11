#include "24cxx.h" 
#include  <bsp.h>	 
//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化IIC接口
void AT24CXX_Init(void)
{
//	IIC_Init();//IIC初始化
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t AT24CXX_ReadOneByte(uint8_t ReadAddr)
{				  
	uint8_t RxBuffer[1];
	while(HAL_I2C_Mem_Read(&hi2c1, (uint16_t)0XA0,(uint16_t)ReadAddr,(uint16_t)I2C_MEMADD_SIZE_8BIT,(uint8_t*)&RxBuffer,1,0x01) != HAL_OK)
		{
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
			{
				Error_Handler();
			}   
		}
//	return RxBuffer[0];
//		uint8_t* Double_ReadI2C(uint8_t REG_Address)
//{
    
//    while(HAL_I2C_Mem_Write(&hi2c1,I2C1_WRITE_ADDRESS,0,I2C_MEMADD_SIZE_8BIT,&ReadAddr,1,10) != HAL_OK)
//    {
//         if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//                {
//                  Error_Handler();
//                }
//    }
//   
//    if(HAL_I2C_Mem_Read(&hi2c1,I2C1_READ_ADDRESS,0,I2C_MEMADD_SIZE_8BIT,(uint8_t*)RxBuffer,1,10) != HAL_OK)
//    {
//        if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//                {
//                  Error_Handler();
//                }
//    }
    return RxBuffer[0];

}
////在AT24CXX指定地址写入一个数据
////WriteAddr  :写入数据的目的地址    
////DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{		
		uint8_t TxBuffer[2]={WriteAddr,DataToWrite};
	
	while(HAL_I2C_Mem_Write(&hi2c1, (uint16_t)0XA0,WriteAddr,I2C_MEMADD_SIZE_8BIT,(uint8_t*)TxBuffer,1,10) != HAL_OK)
		{
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
			{
				Error_Handler();
			}   
		}	
		  while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
  {
  } 
//	uint8_t TxData[2] = {REG_Address,REG_data};
//    while(HAL_I2C_Master_Transmit(&hi2c1,I2C1_WRITE_ADDRESS,(uint8_t*)TxData,2,1000) != HAL_OK)
//    {
//        if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//                {
//                  Error_Handler();
//                }
//    }
//    IIC_Start();  
//	if(EE_TYPE>AT24C16)
//	{
//		IIC_Send_Byte(0XA0);	    //发送写命令
//		IIC_Wait_Ack();
//		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
//	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
//	IIC_Wait_Ack();	   
//    IIC_Send_Byte(WriteAddr%256);   //发送低地址
//	IIC_Wait_Ack(); 	 										  		   
//	IIC_Send_Byte(DataToWrite);     //发送字节							   
//	IIC_Wait_Ack();  		    	   
//    IIC_Stop();//产生一个停止条件 
	HAL_Delay(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
//void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
//{  
//	uint8_t t;	
//	uint8_t TxBuffer[3]={((WriteAddr+t)>>8)&0xff,(WriteAddr+t)&0xff,DataToWrite>>(8*t)};


//  
//	for(t=0;t<Len;t++)
//	{
//				 while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)0XA0, (uint8_t*)TxBuffer, 3, 100)!= HAL_OK)
//  {
//    /* Error_Handler() function is called when Timeout error occurs.
//       When Acknowledge failure occurs (Slave don't acknowledge it's address)
//       Master restarts communication */
//    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//    {
//      Error_Handler();
//    }
////		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
//	}	
//}		
//}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
//uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
//{  	
//	uint8_t t;
//	uint32_t temp=0;
//	uint8_t RxBuffer[1]={ReadAddr+Len-t-1};
//	for(t=0;t<Len;t++)
//	{
//		  while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0XA0, (uint8_t *)RxBuffer, 1, 100) != HAL_OK)
//		{
//    /* Error_Handler() function is called when Timeout error occurs.
//       When Acknowledge failure occurs (Slave don't acknowledge it's address)
//       Master restarts communication */
//			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//			{
//				Error_Handler();
//			}   
//		}
////		temp<<=8;
////		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
//	}
//	return temp;												    
//}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	temp=AT24CXX_ReadOneByte(1);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(1,0X55);
	    temp=AT24CXX_ReadOneByte(1);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{

		while(HAL_I2C_Mem_Read(&hi2c1, (uint16_t)0XA0,ReadAddr,I2C_MEMADD_SIZE_8BIT,(uint8_t *)pBuffer, NumToRead, 100) != HAL_OK)
		{
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
			{
				Error_Handler();
			}   
		}
//	while(NumToRead)
//	{
//		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
//		NumToRead--;
//	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	
	while(HAL_I2C_Mem_Write(&hi2c1, (uint16_t)0XA0,WriteAddr,I2C_MEMADD_SIZE_8BIT,(uint8_t *)pBuffer, NumToWrite, 100) != HAL_OK)
		{
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
			{
				Error_Handler();
			}   
		}
//	while(NumToWrite--)
//	{
//		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
//		WriteAddr++;
//		pBuffer++;
//	}
}








