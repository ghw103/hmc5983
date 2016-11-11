#include "stm32f4xx_hal.h"

//��Ļ��ʼʱ��ʾ��ʽ��ע�⣺��IDelayʱ��ʾ��һ��������������ˢ�µ�
//��ʱ�����ֶ���ˢ�½��������  LCD_WR_REG(0x0007,0x0173);������ʾ
//��Immediatelyʱû�б�ע�͵��ǲ���Ҫ�˹���

/* ѡ��BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ0X60000000~0X63FFFFFF
 * FSMC_A16 ��LCD��DC(�Ĵ���/����ѡ��)��
 * 16 bit => FSMC[24:0]��ӦHADDR[25:1]
 * �Ĵ�������ַ = 0X60000000
 * RAM����ַ = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
 * ��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼��㡣
 */
#define Bank1_LCD_D    ((uint32_t)0x60000002)    //Disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	   //Disp Reg ADDR

//#define Set_Rst GPIOD->BSRR = GPIO_Pin_13;
//#define Clr_Rst GPIOD->BRR  = GPIO_Pin_13;

//#define Lcd_Light_ON   GPIOA->BSRR = GPIO_Pin_1;
//#define Lcd_Light_OFF  GPIOA->BRR  = GPIO_Pin_1;

//������Ϣ�����ַ,ռ33���ֽ�,��1���ֽ����ڱ���ֿ��Ƿ����.����ÿ8���ֽ�һ��,�ֱ𱣴���ʼ��ַ���ļ���С														   
extern uint32_t FONTINFOADDR;	
//�ֿ���Ϣ�ṹ�嶨��
//���������ֿ������Ϣ����ַ����С��
__packed typedef struct 
{
	uint8_t fontok;				//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����
	uint32_t ugbkaddr; 			//unigbk�ĵ�ַ
	uint32_t ugbksize;			//unigbk�Ĵ�С	 
	uint32_t f12addr;			//gbk12��ַ	
	uint32_t gbk12size;			//gbk12�Ĵ�С	 
	uint32_t f16addr;			//gbk16��ַ
	uint32_t gbk16size;			//gbk16�Ĵ�С		 
	uint32_t f24addr;			//gbk24��ַ
	uint32_t gkb24size;			//gbk24�Ĵ�С 
}_font_info; 


extern _font_info ftinfo;	//�ֿ���Ϣ�ṹ��
typedef struct  
{										    
	uint16_t width;			//LCD ���
	uint16_t height;			//LCD �߶�
	uint16_t id;				//LCD ID
	uint8_t  dir;			//���������������ƣ�0��������1��������	
	uint16_t	wramcmd;		//��ʼдgramָ��
	uint16_t  setxcmd;		//����x����ָ��
	uint16_t  setycmd;		//����y����ָ�� 
}_lcd_dev; 

//Lcd��ʼ������ͼ����ƺ���
void DataToWrite(uint16_t data);
void Lcd_Initialize(void);
void LCD_WR_REG(uint16_t Index,uint16_t CongfigTemp);
void Lcd_WR_Start(void);
//Lcd�߼����ƺ���
void Fast_DrawPoint(unsigned int x,unsigned int y,uint16_t color);//����
void Lcd_ColorBox(uint16_t x,uint16_t y,uint16_t xLong,uint16_t yLong,uint16_t Color);
void DrawPixel(uint16_t x, uint16_t y, uint16_t Color);
uint16_t ReadPixel(uint16_t x,uint8_t y);
void LCD_Fill_Pic(uint16_t x, uint16_t y,uint16_t pic_H, uint16_t pic_V, const unsigned char* pic);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//��ʾһ���ַ�
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  						//��ʾһ������
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				//��ʾ ����
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//��ʾһ���ַ���,12/16����
/*���峣����ɫ*/
void Get_HzMat(unsigned char *code,unsigned char *mat,uint8_t size);			//�õ����ֵĵ�����
void Show_Font(uint16_t x,uint16_t y,uint8_t *font,uint8_t size,uint8_t mode);					//��ָ��λ����ʾһ������
void Show_Str(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t*str,uint8_t size,uint8_t mode);	//��ָ��λ����ʾһ���ַ��� 
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
