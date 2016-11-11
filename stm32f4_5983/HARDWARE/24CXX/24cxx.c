#include "24cxx.h" 
#include  <bsp.h>	 
//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
//	IIC_Init();//IIC��ʼ��
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
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
////��AT24CXXָ����ַд��һ������
////WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
////DataToWrite:Ҫд�������
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
//		IIC_Send_Byte(0XA0);	    //����д����
//		IIC_Wait_Ack();
//		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
//	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
//	IIC_Wait_Ack();	   
//    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
//	IIC_Wait_Ack(); 	 										  		   
//	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
//	IIC_Wait_Ack();  		    	   
//    IIC_Stop();//����һ��ֹͣ���� 
	HAL_Delay(10);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
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

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	temp=AT24CXX_ReadOneByte(1);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(1,0X55);
	    temp=AT24CXX_ReadOneByte(1);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
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
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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








