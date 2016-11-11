/******************************************************************************

*重要说明！
在.h文件中，#define Immediately时是立即显示当前画面
而如果#define Delay，则只有在执行了LCD_WR_REG(0x0007,0x0173);
之后才会显示，执行一次LCD_WR_REG(0x0007,0x0173)后，所有写入数
据都立即显示。
#define Delay一般用在开机画面的显示，防止显示出全屏图像的刷新过程
******************************************************************************/
#include "R61408_LCD.h"
#include "font.h" 
#include "string.h"	
#include "w25qxx.h" 
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
//LCD的画笔颜色和背景色	   
uint16_t POINT_COLOR=Yellow;	//画笔颜色
uint16_t BACK_COLOR=0xF81F;  //背景色 

_font_info ftinfo;
//字库区域占用的总扇区数大小(3个字库+unigbk表+字库信息=3238700字节,约占791个W25QXX扇区)
#define FONTSECSIZE	 	791
//字库存放起始地址 
#define FONTINFOADDR 	1024*1024*12 					//Explorer STM32F4是从12M地址以后开始存放字库
/*
 * 函数名：LCD_GPIO_Config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */
// void LCD_GPIO_Config(void)
//{
//}

///*
// * 函数名：LCD_FSMC_Config
// * 描述  ：LCD  FSMC 模式配置
// * 输入  ：无
// * 输出  ：无
// * 调用  ：内部调用        
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
Lcd初始化函数
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
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(uint16_t Index,uint16_t CongfigTemp)
{
	*(__IO uint16_t *) (Bank1_LCD_C) = Index;	
	*(__IO uint16_t *) (Bank1_LCD_D) = CongfigTemp;
}
/************************************************
函数名：Lcd写开始函数
功能：控制Lcd控制引脚 执行写操作
************************************************/
void Lcd_WR_Start(void)
{
*(__IO uint16_t *) (Bank1_LCD_C) = 0x2C;
}
/**********************************************
函数名：Lcd快速画点
功能：选定Lcd上指定的矩形区域


入口参数：x x方向的起始点
          y y方向的起始点
返回值：无
***********************************************/
//快速画点
//x,y:坐标
//color:颜色
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
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的起始点
          Xend   y方向的终止点
          Yend   y方向的终止点
返回值：无
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
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
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
函数名：Lcd图像填充100*100
功能：向Lcd指定位置填充图像
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_Fill_Pic(uint16_t x, uint16_t y,uint16_t pic_H, uint16_t pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x0a); //横屏，从左下角开始，从左到右，从下到上
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
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
} 

//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
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
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
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
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}


//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小
void Get_HzMat(unsigned char *code,unsigned char *mat,uint8_t size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	uint8_t csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
	    return; //结束访问
	}          
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量  		  
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
//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示	   
void Show_Font(uint16_t x,uint16_t y,uint8_t *font,uint8_t size,uint8_t mode)
{
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t dzk[72];   
	uint8_t csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	if(size!=12&&size!=16&&size!=24)return;	//不支持的size
	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//得到点阵数据                          
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
//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式    	   		   
void Show_Str(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t*str,uint8_t size,uint8_t mode)
{					
	uint16_t x0=x;
	uint16_t y0=y;							  	  
    uint8_t bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
                if(x>(x0+width-size/2))//换行
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//越界返回      
		        if(*str==13)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//有效部分写入 
				str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }else//中文 
        {     
            bHz=0;//有汉字库    
            if(x>(x0+width-size))//换行
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//越界返回  						     
	        Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }   
} 

//在指定宽度的中间显示字符串
//如果字符长度超过了len,则用Show_Str显示
//len:指定要显示的宽度			  
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
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		W25QXX_Read((uint8_t*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0XAA)break;
		Delay(200);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}


