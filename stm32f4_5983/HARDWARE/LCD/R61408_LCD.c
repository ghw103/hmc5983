/******************************************************************************

*��Ҫ˵����
��.h�ļ��У�#define Immediatelyʱ��������ʾ��ǰ����
�����#define Delay����ֻ����ִ����LCD_WR_REG(0x0007,0x0173);
֮��Ż���ʾ��ִ��һ��LCD_WR_REG(0x0007,0x0173)������д����
�ݶ�������ʾ��
#define Delayһ�����ڿ����������ʾ����ֹ��ʾ��ȫ��ͼ���ˢ�¹���
******************************************************************************/
#include "R61408_LCD.h"
#include "font.h" 
#include "string.h"	
#include "w25qxx.h" 
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
//LCD�Ļ�����ɫ�ͱ���ɫ	   
uint16_t POINT_COLOR=Yellow;	//������ɫ
uint16_t BACK_COLOR=0xF81F;  //����ɫ 

_font_info ftinfo;
//�ֿ�����ռ�õ�����������С(3���ֿ�+unigbk��+�ֿ���Ϣ=3238700�ֽ�,Լռ791��W25QXX����)
#define FONTSECSIZE	 	791
//�ֿ�����ʼ��ַ 
#define FONTINFOADDR 	1024*1024*12 					//Explorer STM32F4�Ǵ�12M��ַ�Ժ�ʼ����ֿ�
/*
 * ��������LCD_GPIO_Config
 * ����  ������FSMC����LCD��I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����        
 */
// void LCD_GPIO_Config(void)
//{
//}

///*
// * ��������LCD_FSMC_Config
// * ����  ��LCD  FSMC ģʽ����
// * ����  ����
// * ���  ����
// * ����  ���ڲ�����        
// */

//static void LCD_FSMC_Config(void)
//{
//   
//}

volatile static void Delay(__IO uint32_t nCount)
{	
	volatile int i;
	for(i=0;i<0XFFFF;i++)
    for(; nCount != 0; nCount--);
}  
  
//static void LCD_Rst(void)
//{			
//  
//}
static void WriteComm(uint16_t CMD)
{			
	*(__IO uint16_t *) (Bank1_LCD_C) = CMD;
}
static void WriteData(uint16_t tem_data)
{			
	*(__IO uint16_t *) (Bank1_LCD_D) = tem_data;
}

/**********************************************
Lcd��ʼ������
***********************************************/
void Lcd_Initialize(void)
{	
		lcddev.width=480;
		lcddev.height=800;
	
WriteComm(0x11);
Delay(200);

WriteComm(0xB0);
WriteData(0x04);

WriteComm(0xB3);//Frame Memory Access and Interface Setting
WriteData(0x02);
WriteData(0x00);

WriteComm(0xC1);//Panel Driving Setting
WriteData(0x23);
WriteData(0x31);//NL
WriteData(0x99);
WriteData(0x21);
WriteData(0x20);
WriteData(0x00);
WriteData(0x10);//DIVI
WriteData(0x28);//RTN
WriteData(0x0C);//BP
WriteData(0x0A);//FP
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x21);
WriteData(0x01);

WriteComm(0xC2);//Display V-Timing Setting
WriteData(0x00);
WriteData(0x06);
WriteData(0x06);
WriteData(0x01);
WriteData(0x03);
WriteData(0x00);

WriteComm(0xC8);//GAMMA
WriteData(0x01);
WriteData(0x0A);
WriteData(0x12);
WriteData(0x1C);
WriteData(0x2B);
WriteData(0x45);
WriteData(0x3F);
WriteData(0x29);
WriteData(0x17);
WriteData(0x13);
WriteData(0x0F);
WriteData(0x04);

WriteData(0x01);
WriteData(0x0A);
WriteData(0x12);
WriteData(0x1C);
WriteData(0x2B);
WriteData(0x45);
WriteData(0x3F);
WriteData(0x29);
WriteData(0x17);
WriteData(0x13);
WriteData(0x0F);
WriteData(0x04);

WriteComm(0xC9);//GAMMA
WriteData(0x01);
WriteData(0x0A);
WriteData(0x12);
WriteData(0x1C);
WriteData(0x2B);
WriteData(0x45);
WriteData(0x3F);
WriteData(0x29);
WriteData(0x17);
WriteData(0x13);
WriteData(0x0F);
WriteData(0x04);

WriteData(0x01);
WriteData(0x0A);
WriteData(0x12);
WriteData(0x1C);
WriteData(0x2B);
WriteData(0x45);
WriteData(0x3F);
WriteData(0x29);
WriteData(0x17);
WriteData(0x13);
WriteData(0x0F);
WriteData(0x04);

WriteComm(0xCA);//GAMMA
WriteData(0x01);
WriteData(0x0A);
WriteData(0x12);
WriteData(0x1C);
WriteData(0x2B);
WriteData(0x45);
WriteData(0x3F);
WriteData(0x29);
WriteData(0x17);
WriteData(0x13);
WriteData(0x0F);
WriteData(0x04);

WriteData(0x01);
WriteData(0x0A);
WriteData(0x12);
WriteData(0x1C);
WriteData(0x2B);
WriteData(0x45);
WriteData(0x3F);
WriteData(0x29);
WriteData(0x17);
WriteData(0x13);
WriteData(0x0F);
WriteData(0x04);

WriteComm(0xD0);//Power Setting (Charge Pump Setting)
WriteData(0x99);//DC
WriteData(0x03);
WriteData(0xCE);
WriteData(0xA6);
WriteData(0x00);//CP or SR
WriteData(0x43);//VC3, VC2
WriteData(0x20);
WriteData(0x10);
WriteData(0x01);
WriteData(0x00);
WriteData(0x01);
WriteData(0x01);
WriteData(0x00);
WriteData(0x03);
WriteData(0x01);
WriteData(0x00);

WriteComm(0xD3);//Power Setting for Internal Mode
WriteData(0x33);//AP

WriteComm(0xD5);//VPLVL/VNLVL Setting
WriteData(0x2A);
WriteData(0x2A);

WriteComm(0xD6);//
WriteData(0xA8);//

WriteComm(0xD6);//
WriteData(0x01);//

WriteComm(0xDE);//VCOMDC Setting
WriteData(0x01);
WriteData(0x4F);

WriteComm(0xE6);//VCOMDC Setting
WriteData(0x4F);

WriteComm(0xFA);//VDC_SEL Setting
WriteData(0x03);

Delay(100);

WriteComm(0x2A);
WriteData(0x00);
WriteData(0x00);
WriteData(0x01);
WriteData(0xDF);

WriteComm(0x2B);
WriteData(0x00);
WriteData(0x00);
WriteData(0x03);
WriteData(0x1F);

WriteComm(0x36);
WriteData(0x60);

WriteComm(0x3A);
WriteData(0x55);

WriteComm(0x29);
Delay(20);

WriteComm(0x2C); 
Delay(10); 
Lcd_ColorBox(0,0,800,480,0x0f6F);
// printf("ReadPixel=%04x\r\n",ReadPixel(8, 8));
// DrawPixel(10, 10, 0xaaaa);
// printf("ReadPixel=%04x\r\n",ReadPixel(10, 10));
// DrawPixel(10, 11, 0XFFFF);
// printf("ReadPixel=%04x\r\n",ReadPixel(10, 11));
// DrawPixel(10, 12, 0X00);
// printf("ReadPixel=%04x\r\n",ReadPixel(10, 12));
// while(1);
// // while(1)
// // 	{
// // 		Lcd_ColorBox(0,0,480,800,Yellow);Delay(5000000);
// // 		Lcd_ColorBox(0,0,480,800,Blue);Delay(5000000);
// // 		Lcd_ColorBox(0,0,480,800,Red);Delay(5000000);
// // 		Lcd_ColorBox(0,0,480,800,Green);Delay(5000000);
// // 		Lcd_ColorBox(0,0,480,800,Magenta);Delay(5000000);
// // 		Lcd_ColorBox(0,0,480,800,Cyan);Delay(5000000);
// // 		
// // // 		Lcd_ColorBox(0,0,800,480,Yellow);Delay(5000000);
// // // 		Lcd_ColorBox(0,0,800,480,Blue);Delay(5000000);
// // // 		Lcd_ColorBox(0,0,800,480,Red);Delay(5000000);
// // // 		Lcd_ColorBox(0,0,800,480,Green);Delay(5000000);
// // // 		Lcd_ColorBox(0,0,800,480,Magenta);Delay(5000000);
// // // 		Lcd_ColorBox(0,0,800,480,Cyan);Delay(5000000);
// // // 		Lcd_ColorBox(0,0,800,480,White);Delay(5000000);
// // 		WriteComm(0x36);  
// // WriteData(0x20);
// // 		LCD_Fill_Pic(0,0,320,480, gImage_MM_T035);Delay(50000000);
// // 		WriteComm(0x36);  
// // WriteData(0x10);
// // 	}
}
/******************************************
��������Lcdд�����
���ܣ���Lcdָ��λ��д��Ӧ�����������
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
******************************************/
void LCD_WR_REG(uint16_t Index,uint16_t CongfigTemp)
{
	*(__IO uint16_t *) (Bank1_LCD_C) = Index;	
	*(__IO uint16_t *) (Bank1_LCD_D) = CongfigTemp;
}
/************************************************
��������Lcdд��ʼ����
���ܣ�����Lcd�������� ִ��д����
************************************************/
void Lcd_WR_Start(void)
{
*(__IO uint16_t *) (Bank1_LCD_C) = 0x2C;
}
/**********************************************
��������Lcd���ٻ���
���ܣ�ѡ��Lcd��ָ���ľ�������


��ڲ�����x x�������ʼ��
          y y�������ʼ��
����ֵ����
***********************************************/
//���ٻ���
//x,y:����
//color:��ɫ
void Fast_DrawPoint(unsigned int x,unsigned int y,uint16_t color)
{	
	*(__IO uint16_t *) (Bank1_LCD_C) =0x2a;  
	*(__IO uint16_t *) (Bank1_LCD_D) = x>>8;  
	*(__IO uint16_t *) (Bank1_LCD_D) =x&0xff; 

	*(__IO uint16_t *) (Bank1_LCD_C) =0x2b;  
	*(__IO uint16_t *) (Bank1_LCD_D) =y>>8;  
	*(__IO uint16_t *) (Bank1_LCD_D) =y&0xff; 

	*(__IO uint16_t *) (Bank1_LCD_C) =0x2c;  
	*(__IO uint16_t *) (Bank1_LCD_D) = color;
}
/**********************************************
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart��yStart��Xend��Yend������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ʼ��
          Xend   y�������ֹ��
          Yend   y�������ֹ��
����ֵ����
***********************************************/
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{

	*(__IO uint16_t *) (Bank1_LCD_C) =0x2a;  	
	WriteData(Xstart>>8);
	WriteData(Xstart&0xff);
	WriteData(Xend>>8);
	WriteData(Xend&0xff);
	
	*(__IO uint16_t *) (Bank1_LCD_C) =0x2b;  	
	WriteData(Ystart>>8);
	WriteData(Ystart&0xff);
	WriteData(Yend>>8);
	WriteData(Yend&0xff);

	*(__IO uint16_t *) (Bank1_LCD_C) =0x2c;  
	
}
/**********************************************
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart�� yStart������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ֹ��
          xLong Ҫѡ�����ε�x���򳤶�
          yLong  Ҫѡ�����ε�y���򳤶�
����ֵ����
***********************************************/
void Lcd_ColorBox(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong,uint16_t Color)
{
	uint32_t temp;

	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		*(__IO uint16_t *) (Bank1_LCD_D) = Color;
	}
}

/******************************************
��������Lcdͼ�����100*100
���ܣ���Lcdָ��λ�����ͼ��
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
******************************************/
void LCD_Fill_Pic(uint16_t x, uint16_t y,uint16_t pic_H, uint16_t pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x0a); //�����������½ǿ�ʼ�������ң����µ���
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO uint16_t *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0xaa);
}

//void DrawPixel(uint16_t Xstart, uint16_t Ystart, uint16_t Color)
//{
// 
//	*(__IO uint16_t *) (Bank1_LCD_D) = Color;
//}

uint16_t ReadPixel(uint16_t x,uint8_t y)
{
	uint16_t aData[3];
  uint16_t Index;

//  GUI_USE_PARA(LayerIndex);
  //
  // Switch to read mode
  //
	
  WriteComm(0x2E);
  //
  // Dummy- and data read
  //
	 
  aData[0]=*(__IO uint16_t *) (Bank1_LCD_D);
	aData[1]=*(__IO uint16_t *) (Bank1_LCD_D);
	aData[2]=*(__IO uint16_t *) (Bank1_LCD_D);
//	 WriteComm(0x2E);
//	aData[0]=*(__IO uint16_t *) (Bank1_LCD_D);
//	aData[1]=*(__IO uint16_t *) (Bank1_LCD_D);
//	aData[2]=*(__IO uint16_t *) (Bank1_LCD_D);
  //
  // Convert to index
  //
  Index = (aData[2] >> 11) | ((aData[1] & 0xfc) << 3) | (aData[1] & 0xf800);
		   
//      Index  =  aData[1] & 0xf800;        // blue
//      Index |= (aData[1] & 0x00fc) <<  3; // green
//     
//      Index |= (aData[2] & 0xf800) >> 11; // red
//		printf("RIN=%04x\r\n",Index);
	return Index;
}

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
} 

//m^n����
//����ֵ:m^n�η�.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}


//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
void Get_HzMat(unsigned char *code,unsigned char *mat,uint8_t size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	uint8_t csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//�������
	    return; //��������
	}          
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����  		  
	switch(size)
	{
		case 12:
			W25QXX_Read(mat,foffset+ftinfo.f12addr,csize);
			break;
		case 16:
			W25QXX_Read(mat,foffset+ftinfo.f16addr,csize);
			break;
		case 24:
			W25QXX_Read(mat,foffset+ftinfo.f24addr,csize);
			break;
			
	}     												    
}  
//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	   
void Show_Font(uint16_t x,uint16_t y,uint8_t *font,uint8_t size,uint8_t mode)
{
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t dzk[72];   
	uint8_t csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size
	Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)Fast_DrawPoint(x,y,BACK_COLOR); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t*str,uint8_t size,uint8_t mode)
{					
	uint16_t x0=x;
	uint16_t y0=y;							  	  
    uint8_t bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//���� 
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
				str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(x>(x0+width-size))//����
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//Խ�緵��  						     
	        Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }   
} 

//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
void Show_Str_Mid(uint16_t x,uint16_t y,uint8_t*str,uint8_t size,uint8_t len)
{
	uint16_t strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
	}
} 

 uint8_t font_init(void)
{		
	uint8_t t=0;
//	W25QXX_Init();  
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		W25QXX_Read((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break;
		Delay(200);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}


