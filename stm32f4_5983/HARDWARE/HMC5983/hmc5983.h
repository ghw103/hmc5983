#ifndef __5983_H
#define __5983_H		

#include "stm32f4xx_hal.h" 
#include  <bsp.h>
//#include "string.h"
#define	HMC5983_CS 		    PEout(2)  		//W25QXXµÄÆ¬Ñ¡ÐÅºÅ
#define	HMC5983_CS2 		PAout(0)
#define	HMC5983_CS3 		PAout(1)
#define	HMC5983_CS4 		PAout(2)
#define	HMC5983_CS5 		PAout(3)
#define	HMC5983_CS6 		PAout(4)
#define	HMC5983_CS7 		PAout(5)
//#define HMC5883L_Address 0x1E
#define ConfigurationRegisterA 0x00
#define ConfigurationRegisterB 0x01
#define ModeRegister 0x02
#define DataRegisterBegin 0x03

#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03

//*****************************************************************************
//
// The following are defines for the hmc5983 register addresses.
//
//*****************************************************************************
#define HMC5983_O_CONFIG_A     0x00 //Configuration Register A 
#define HMC5983_O_CONFIG_B     0x01 //Configuration Register B
#define HMC5983_O_MODE         0x02 //Mode Register
#define HMC5983_O_XOUT_H       0x03 //Data Output X MSB Register
#define HMC5983_O_XOUT_L       0x04 //Data Output X LSB Register
#define HMC5983_O_ZOUT_H       0x05 //Data Output Z MSB Register
#define HMC5983_O_ZOUT_L       0x06 //Data Output Z LSB Register
#define HMC5983_O_YOUT_H       0x07 //Data Output Y MSB Register
#define HMC5983_O_YOUT_L       0x08 //Data Output Y LSB Register
#define HMC5983_O_STATUS       0x09 //Status Register
#define HMC5983_O_ID_A         0x0A //Identification Register A
#define HMC5983_O_ID_B         0x0B //Identification Register B
#define HMC5983_O_ID_C         0x0C //Identification Register C 
#define HMC5983_O_TS_H         0x31 //Temperature Output MSB Register
#define HMC5983_O_TS_L         0x32 //Temperature Output LSB Register
//*****************************************************************************
//
// The following are defines for the bit fields in the HMC_O_CONFIG_A
// register.
//
//*****************************************************************************
#define HMC5983_CONFIG_A_DEFAULT        0x70

#define HMC5983_CONFIG_A_TS_ENABLE      0x80

#define HMC5983_CONFIG_A_AVG_1          0x00
#define HMC5983_CONFIG_A_AVG_2          0x20
#define HMC5983_CONFIG_A_AVG_4          0x40
#define HMC5983_CONFIG_A_AVG_8          0x60

#define HMC5983_CONFIG_A_HZ_dot75       0x00
#define HMC5983_CONFIG_A_HZ_1dot5       0x04
#define HMC5983_CONFIG_A_HZ_3           0x08
#define HMC5983_CONFIG_A_HZ_7dot5       0x0C
#define HMC5983_CONFIG_A_HZ_15          0x10
#define HMC5983_CONFIG_A_HZ_30          0x14
#define HMC5983_CONFIG_A_HZ_75          0x18
#define HMC5983_CONFIG_A_HZ_160         0x1C

#define HMC5983_CONFIG_A_MEASURE_NORMAL   0x00
#define HMC5983_CONFIG_A_MEASURE_PBIAS    0x01
#define HMC5983_CONFIG_A_MEASURE_NBIAS    0x02
#define HMC5983_CONFIG_A_MEASURE_TS       0x03

//*****************************************************************************
//
// The following are defines for the bit fields in the HMC_O_CONFIG_B
// register.
//
//*****************************************************************************
#define HMC5983_CONFIG_B_DEFAULT    0x20

#define HMC5983_CONFIG_B_GAIN_M     0xE0

#define HMC5983_CONFIG_B_GAIN_1370  0x00
#define HMC5983_CONFIG_B_GAIN_1090  0x20
#define HMC5983_CONFIG_B_GAIN_820   0x40
#define HMC5983_CONFIG_B_GAIN_660   0x60
#define HMC5983_CONFIG_B_GAIN_440   0x80
#define HMC5983_CONFIG_B_GAIN_390   0xA0
#define HMC5983_CONFIG_B_GAIN_330   0xC0
#define HMC5983_CONFIG_B_GAIN_230   0xE0

#define HMC5983_CONFIG_B_GAIN_S     5

//*****************************************************************************
//
// The following are defines for the bit fields in the HMC_O_MODE
// register.
//
//*****************************************************************************
#define HMC5983_MODE_HS             0x80

#define HMC5983_MODE_LP             0x20

#define HMC5983_MODE_CONTINUOUS     0x00
#define HMC5983_MODE_SINGLE         0x01
#define HMC5983_MODE_IDLE           0x02  //????

//*****************************************************************************
//
// The following are defines for the bit fields in the HMC_O_STATUS
// register.
// It is only read
//
//*****************************************************************************
#define HMC5983_STATUS_DOW     0x10

#define HMC5983_STATUS_LOCK    0x02
#define HMC5983_STATUS_RDY     0x01

//*****************************************************************************
//
// The following are defines for the bit fields in the HMC_O_ID_?
// register.
//
//*****************************************************************************
#define HMC5983_ID_A   0x48
// ASCII H
#define HMC5983_ID_B   0x34
// ASCII 4
#define HMC5983_ID_C   0x33
// ASCII 3

//*****************************************************************************
/*
//?? mGa
static const float g_fHMC5883LFactors[] =
{
    0.73,                              // Range = +/- 880mGa        
    0.92,                              // Range = +/- 1300mGa
    1.22,                              // Range = +/- 1900mGa
    1.52,                              // Range = +/- 2500mGa
    2.27,                              // Range = +/- 4000mGa
    2.56,                              // Range = +/- 4700mGa
    3.03,                              // Range = +/- 5600mGa
    4.35                               // Range = +/- 8100mGa
};
*/
//?? Ga
static const float g_fHMC5883LFactors[] =
{
    0.00073,                           // Range = +/- 0.88Ga  
    0.00092,                           // Range = +/- 1.3Ga
    0.00122,                           // Range = +/- 1.9Ga
    0.00152,                           // Range = +/- 2.5Ga
    0.00227,                           // Range = +/- 4.0Ga
    0.00256,                           // Range = +/- 4.7Ga
    0.00303,                           // Range = +/- 5.6Ga
    0.00435                            // Range = +/- 8.1Ga
};

typedef struct {
    int16_t x[8];
    int16_t y[8];
    int16_t z[8];
    float kx[8];
    float ky[8];
    float kz[8];
    int16_t a0x[8];
    int16_t a0y[8];
    int16_t a0z[8];
    int16_t a1x[8];   
    int16_t a1y[8];
    int16_t a1z[8]; 
    uint16_t dif[8];    
} mag;

extern mag hmc5983;
void hmc5983_init(void);
void hmc5983_redbuf(int16_t *mgx,int16_t *mgy,int16_t *mgz,uint8_t cs)	;


#endif


