#include "hmc5983.h" 
#include "math.h"
#define Dummy_Byte  0xff
mag hmc5983;
 void hmc5983_calibrate();
void hmc_5983_cs(uint8_t cs)
{
switch (cs)
{
    case 0:
        HMC5983_CS=1;
        HMC5983_CS2=1;
        HMC5983_CS3=1;
        HMC5983_CS4=1;
        HMC5983_CS5=1;
        HMC5983_CS6=1;
        HMC5983_CS7=1;
        break;
	case 1:
        HMC5983_CS=0;
        HMC5983_CS2=1;
        HMC5983_CS3=1;
        HMC5983_CS4=1;
        HMC5983_CS5=1;
        HMC5983_CS6=1;
        HMC5983_CS7=1;
		break;
	case 2:
        HMC5983_CS=1;
        HMC5983_CS2=0;
        HMC5983_CS3=1;
        HMC5983_CS4=1;
        HMC5983_CS5=1;
        HMC5983_CS6=1;
        HMC5983_CS7=1;
		break;
    case 3:
        HMC5983_CS=1;
        HMC5983_CS2=1;
        HMC5983_CS3=0;
        HMC5983_CS4=1;
        HMC5983_CS5=1;
        HMC5983_CS6=1;
        HMC5983_CS7=1;
		break;
	case 4:
        HMC5983_CS=1;
        HMC5983_CS2=1;
        HMC5983_CS3=1;
        HMC5983_CS4=0;
        HMC5983_CS5=1;
        HMC5983_CS6=1;
        HMC5983_CS7=1;
		break;
    case 5:
        HMC5983_CS=1;
        HMC5983_CS2=1;
        HMC5983_CS3=1;
        HMC5983_CS4=1;
        HMC5983_CS5=0;
        HMC5983_CS6=1;
        HMC5983_CS7=1;
		break;
	case 6:
        HMC5983_CS=1;
        HMC5983_CS2=1;
        HMC5983_CS3=1;
        HMC5983_CS4=1;
        HMC5983_CS5=1;
        HMC5983_CS6=0;
        HMC5983_CS7=1;
		break;
    case 7:
        HMC5983_CS=1;
        HMC5983_CS2=1;
        HMC5983_CS3=1;
        HMC5983_CS4=1;
        HMC5983_CS5=1;
        HMC5983_CS6=1;
        HMC5983_CS7=0;
		break;
	default:
        HMC5983_CS=1;
        HMC5983_CS2=1;
        HMC5983_CS3=1;
        HMC5983_CS4=1;
        HMC5983_CS5=1;
        HMC5983_CS6=1;
        HMC5983_CS7=1;
		break;
}
return;

}
uint8_t spi2_ReadWriteByte(uint8_t TxData)
{		 			 
 
  while ((SPI1->SR & SPI_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI1->DR = TxData; //通过外设SPIx发送一个byte  数据
		
  while ((SPI1->SR & SPI_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI1->DR;; //返回通过SPIx最近接收的数据	
 		    
}

void hmc5983_WriteByte(uint8_t Address,uint8_t byte,uint8_t cs)
{

        hmc_5983_cs(cs);
    spi2_ReadWriteByte((Address&0X7F));   //?
    spi2_ReadWriteByte(byte);
        hmc_5983_cs(0);
}


uint8_t hmc5983_ReadByte(uint8_t Address)
{
   uint8_t temp=0;
//   HMC5983_CS=0;
   spi2_ReadWriteByte(Address|0x80);   //?
   temp=spi2_ReadWriteByte(Dummy_Byte);
//   HMC5983_CS=1;
   return temp;
}

void hmc5983_ReadBuf(uint8_t Address,uint8_t *Buf,uint8_t len,uint8_t cs)
{
    uint8_t i=0;
//       HMC5983_CS=0; 
    for(i=0;i<len;i++)
    {
// HMC5983_CS2=0; 
        hmc_5983_cs(cs);
//          HAL_Delay(10);
      spi2_ReadWriteByte(((Address+i)|0x80));
      Buf[i]=spi2_ReadWriteByte(Dummy_Byte);
//  HMC5983_CS2=1;
        hmc_5983_cs(0);
    }
//          HMC5983_CS=1;
//    
}
#include "WM.h"
//////////////////////////////////
void hmc5983_init(void)
{
    hmc_5983_cs(0);
    unsigned char MPU_buff[6]={0};
    HAL_Delay(100);
    hmc5983_calibrate();
    uint8_t i;    
    for(i=1;i<8;i++)
    {
        HAL_Delay(100);
        hmc5983_WriteByte(ConfigurationRegisterA,0xf8,i);    //????,1??? 30HZ 0x14
        hmc5983_WriteByte(ConfigurationRegisterB,0xe0,i);    //8.1Ga
        hmc5983_WriteByte(ModeRegister,0x00,i);
    }
        for(i=1;i<8;i++)
    {
    hmc5983_ReadBuf(DataRegisterBegin,MPU_buff,6,i);

    hmc5983.x[i]=(((int16_t)MPU_buff[0]) << 8) | MPU_buff[1];
    hmc5983.y[i]=(((int16_t)MPU_buff[2]) << 8) | MPU_buff[3];
    hmc5983.z[i]=(((int16_t)MPU_buff[4]) << 8) | MPU_buff[5];
    
    hmc5983.a0x[i]=(int16_t)(hmc5983.x[i]* hmc5983.kx[i]);
    hmc5983.a0y[i]=(int16_t)(hmc5983.y[i]* hmc5983.ky[i]);
    hmc5983.a0z[i]=(int16_t)(hmc5983.z[i]* hmc5983.kz[i]); 
   HAL_Delay(50); 
//    hmc5983.a0x[i]=((((int16_t)MPU_buff1[0]) << 8) | MPU_buff1[1])*hmc5983.kx[i];
//    hmc5983.a0y[i]=((((int16_t)MPU_buff1[2]) << 8) | MPU_buff1[3])*hmc5983.ky[i];
//    hmc5983.a0z[i]=((((int16_t)MPU_buff1[4]) << 8) | MPU_buff1[5])*hmc5983.kz[i];
    }
}
 void hmc5983_calibrate()
{

    char buf[20]={0};
    unsigned char MPU_buff[6]={0};
    uint8_t i,j,sum=0;
    int16_t x,y,z;
    int32_t sumx=0,sumy=0,sumz=0;

    for(j=1;j<8;j++)
    {
     hmc5983_WriteByte(ConfigurationRegisterA,0xf9,j);    //75hz 8次平均 温度补偿 自测
     hmc5983_WriteByte(ConfigurationRegisterB,0xe0,j);    //2.5Ga
     hmc5983_WriteByte(ModeRegister,0x00,j);
        for  (i=0;i<10;i++){ hmc5983_ReadBuf(DataRegisterBegin,MPU_buff,6,j);
                                HAL_Delay(20);
                            }
    for  (i=0;i<10;i++){
    hmc5983_ReadBuf(DataRegisterBegin,MPU_buff,6,j);
        
    hmc5983.x[j]=(((int16_t)MPU_buff[0]) << 8) | MPU_buff[1];
    hmc5983.y[j]=(((int16_t)MPU_buff[2]) << 8) | MPU_buff[3];
    hmc5983.z[j]=(((int16_t)MPU_buff[4]) << 8) | MPU_buff[5];
    printf("x:%2d",hmc5983.x[j]);
    printf("y:%2d",hmc5983.y[j]);
    printf("z:%d\n",hmc5983.z[j]);    
        if((hmc5983.x[j]*g_fHMC5883LFactors[7])<1.35f&&(hmc5983.x[j]*g_fHMC5883LFactors[7])>0.7f\
           &&(hmc5983.y[j]*g_fHMC5883LFactors[7])<1.35f&&(hmc5983.y[j]*g_fHMC5883LFactors[7])>0.7f\
           &&(hmc5983.z[j]*g_fHMC5883LFactors[7])<1.35f&&(hmc5983.z[j]*g_fHMC5883LFactors[7])>0.7f)
        {
         sumx+=hmc5983.x[j];
         sumy+=hmc5983.y[j];
         sumz+=hmc5983.z[j];   
         sum++; 
         GUI_DispDecAt(sum,100,80,2);        
        }
        HAL_Delay(20);
            }
            
//    hmc5983.kx[j]=(1.08f/((sumx/sum)*g_fHMC5883LFactors[7]));
//    hmc5983.ky[j]=(1.08f/((sumy/sum)*g_fHMC5883LFactors[7]));
//    hmc5983.kz[j]=(1.16f/((sumz/sum)*g_fHMC5883LFactors[7]));

    hmc5983.kx[j]=(1080.0f/(sumx/sum));
    hmc5983.ky[j]=(1080.0f/(sumy/sum));
    hmc5983.kz[j]=(1160.0f/(sumz/sum));
            
    sumx=sumy=sumz=sum=0;
    //GUI_DispFloat(hmc5983.kx[j],4);
     }

}	
void hmc5983_redbuf(int16_t *mgx,int16_t *mgy,int16_t *mgz,uint8_t cs)	
{
unsigned char MPU_buff[6]={0};
uint8_t i;
    char buf[20]={0};
//hmc5983_WriteByte(ModeRegister,  0x01);
//hmc_5983_cs(0);
for(i=1;i<8;i++)
{

    hmc5983_ReadBuf(DataRegisterBegin,MPU_buff,6,i);
    hmc5983.x[i]=(((int16_t)MPU_buff[0]) << 8) | MPU_buff[1];
    hmc5983.y[i]=(((int16_t)MPU_buff[2]) << 8) | MPU_buff[3];
    hmc5983.z[i]=(((int16_t)MPU_buff[4]) << 8) | MPU_buff[5];
    
    hmc5983.a1x[i]=(int16_t)(hmc5983.x[i]* hmc5983.kx[i]);
    hmc5983.a1y[i]=(int16_t)(hmc5983.y[i]* hmc5983.ky[i]);
    hmc5983.a1z[i]=(int16_t)(hmc5983.z[i]* hmc5983.kz[i]);
    
    hmc5983.dif[i]=sqrt((sqrt((hmc5983.a1x[i]*hmc5983.a1x[i])+(hmc5983.a1y[i]*hmc5983.a1y[i])+(hmc5983.a1z[i]*hmc5983.a1z[i]))\
                    -sqrt((hmc5983.a0x[i]*hmc5983.a0x[i])+(hmc5983.a0y[i]*hmc5983.a0y[i])+(hmc5983.a0z[i]*hmc5983.a0z[i])))\
                       *(sqrt((hmc5983.a1x[i]*hmc5983.a1x[i])+(hmc5983.a1y[i]*hmc5983.a1y[i])+(hmc5983.a1z[i]*hmc5983.a1z[i]))\
                    -sqrt((hmc5983.a0x[i]*hmc5983.a0x[i])+(hmc5983.a0y[i]*hmc5983.a0y[i])+(hmc5983.a0z[i]*hmc5983.a0z[i]))));
}


    

//printf("x:%2d",*mgx);
//printf("y:%2d",*mgy);
//printf("z:%d\n",*mgz);
}
	

void show(uint8_t mag)
{



}

