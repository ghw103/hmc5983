#include "stm32f4xx_hal.h"

//屏幕开始时显示方式，注意：当IDelay时显示第一幅画面是逐像素刷新的
//此时必须手动在刷新结束后加上  LCD_WR_REG(0x0007,0x0173);才能显示
//当Immediately时没有被注释掉是不需要此过程

/* 选择BANK1-BORSRAM1 连接 TFT，地址范围为0X60000000~0X63FFFFFF
 * FSMC_A16 接LCD的DC(寄存器/数据选择)脚
 * 16 bit => FSMC[24:0]对应HADDR[25:1]
 * 寄存器基地址 = 0X60000000
 * RAM基地址 = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
 * 当选择不同的地址线时，地址要重新计算。
 */
#define Bank1_LCD_D    ((uint32_t)0x60000002)    //Disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	   //Disp Reg ADDR

//#define Set_Rst GPIOD->BSRR = GPIO_Pin_13;
//#define Clr_Rst GPIOD->BRR  = GPIO_Pin_13;

//#define Lcd_Light_ON   GPIOA->BSRR = GPIO_Pin_1;
//#define Lcd_Light_OFF  GPIOA->BRR  = GPIO_Pin_1;

//字体信息保存地址,占33个字节,第1个字节用于标记字库是否存在.后续每8个字节一组,分别保存起始地址和文件大小														   
extern uint32_t FONTINFOADDR;	
//字库信息结构体定义
//用来保存字库基本信息，地址，大小等
__packed typedef struct 
{
	uint8_t fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在
	uint32_t ugbkaddr; 			//unigbk的地址
	uint32_t ugbksize;			//unigbk的大小	 
	uint32_t f12addr;			//gbk12地址	
	uint32_t gbk12size;			//gbk12的大小	 
	uint32_t f16addr;			//gbk16地址
	uint32_t gbk16size;			//gbk16的大小		 
	uint32_t f24addr;			//gbk24地址
	uint32_t gkb24size;			//gbk24的大小 
}_font_info; 


extern _font_info ftinfo;	//字库信息结构体
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				//LCD ID
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令 
}_lcd_dev; 

//Lcd初始化及其低级控制函数
void DataToWrite(uint16_t data);
void Lcd_Initialize(void);
void LCD_WR_REG(uint16_t Index,uint16_t CongfigTemp);
void Lcd_WR_Start(void);
//Lcd高级控制函数
void Fast_DrawPoint(unsigned int x,unsigned int y,uint16_t color);//画点
void Lcd_ColorBox(uint16_t x,uint16_t y,uint16_t xLong,uint16_t yLong,uint16_t Color);
void DrawPixel(uint16_t x, uint16_t y, uint16_t Color);
uint16_t ReadPixel(uint16_t x,uint8_t y);
void LCD_Fill_Pic(uint16_t x, uint16_t y,uint16_t pic_H, uint16_t pic_V, const unsigned char* pic);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//显示一个字符
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  						//显示一个数字
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				//显示 数字
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//显示一个字符串,12/16字体
/*定义常见颜色*/
void Get_HzMat(unsigned char *code,unsigned char *mat,uint8_t size);			//得到汉字的点阵码
void Show_Font(uint16_t x,uint16_t y,uint8_t *font,uint8_t size,uint8_t mode);					//在指定位置显示一个汉字
void Show_Str(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t*str,uint8_t size,uint8_t mode);	//在指定位置显示一个字符串 
void Show_Str_Mid(uint16_t x,uint16_t y,uint8_t*str,uint8_t size,uint8_t len);
 uint8_t font_init(void);
 
 
 
 
 
 
 
 
 
 
 
#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
