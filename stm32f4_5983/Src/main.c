/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */
#include "sram.h"	 
#include "R61408_LCD.h"
#include "w25qxx.h" 
#include "24cxx.h" 
//#include  <includes.h>
#include "WM.h"
#include "hmc5983.h" 
//#include "GUIDEMO.h"
#include "Wifi.h"
#include <string.h>
#include  "lcd_log.h"
#include  <bsp.h>
#include  "progbar.h"
//#include "GUIDEMO.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO ITStatus UartReady = RESET;
/* Private variables ---------------------------------------------------------*/
                                                               /* ----------------- APPLICATION GLOBALS -------------- */
//static  OS_TCB   AppTaskStartTCB;
//static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
//                                                                /* ------------ FLOATING POINT TEST TASK -------------- */
//static  OS_TCB       App_TaskledTCB;
//static  CPU_STK      App_TaskledStk[APP_CFG_TASK_led_STK_SIZE];


//int fputc(int ch, FILE *f)
//{
//  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&ch, 1)!= HAL_OK)
//  {
//    /* Transfer error in transmission process */
//    Error_Handler();    
//  }
//  /*##-6- Wait for the end of the transfer ###################################*/  
//  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
//  {
//  }
////HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF); 
//return ch;
//}

#define SIZE sizeof(TEXT_Buffer)	 
uint16_t ADCxConvertedValue2[40]={0};	
uint16_t ADCxConvertedValue[5]={0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define ADDR_24LCxx_Write 0xA0
#define ADDR_24LCxx_Read 0xA1
#define BufferSize 10
uint8_t WriteBuffer[BufferSize],ReadBuffer[BufferSize];
int16_t i,x,y,z;
/* USER CODE END PV */

 FRESULT res;  /* FatFs function common result code */
	UINT br, bw;  

  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "\r\n"; /* File write buffer */
  uint8_t rtext[100];  
 uint8_t rademe[1024];  /* File read buffer */
BYTE sample_buffer1[] = "This is STM32 working with FatFs \r\n"; 
BYTE buffer[4096]={0}; 
/* USER CODE END 0 */
WM_HWIN CreateWindow(void);

extern  WM_HWIN hItem;


int main(void)
{

  /* USER CODE BEGIN 1 */
RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	char buf[20]={0};
		
//	 	DIR picdir;	 		//图片目录
//	FILINFO picfileinfo;//文件信息
          					/* File R/W count */
//		uint8_t datatemp[SIZE];	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FMC_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_SDIO_SD_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_RTC_Init();
  MX_FATFS_Init();

  /* USER CODE BEGIN 2 */
	 /* Enable the CRC Module */
  __HAL_RCC_CRC_CLK_ENABLE();
/* 窗口自动使用存储设备*/

    WM_EnableMemdev(WM_HBKWIN);
	 WM_SetCreateFlags(WM_CF_MEMDEV);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	TIM3->CCR4 = 800;
	



//	GUI_Clear();

	#if GUI_WINSUPPORT
	WM_SetCreateFlags(WM_CF_MEMDEV);
#endif
Lcd_Initialize();
	GUI_Init();
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
    GUI_Clear();
        font_init();
        HAL_Delay(500);
	hmc5983_init();
	//GUI_SetFont(GUI_FONT_24_1);
#if GUI_WINSUPPORT
	WM_MULTIBUF_Enable(1);
#endif
//	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    
	WM_HWIN hWin;
	hWin = CreateWindow();
   // WM_SetCallback(WM_HBKWIN, _cbDialog);
    GUI_CURSOR_Show();
    GUI_Delay(100);
    
    //WM_Paint(hWin);	


    	while(font_init()){
	}
//GUI_Clear();
//   GUI_CURSOR_Show();
/* USER CODE BEGIN 2 */
//printf("\r\n***************I2C Exampleabcdefghigklmnopqrstuvwxyzabcdefg");
//for(i=0; i<256; i++)
//    WriteBuffer[i]=0x08;    /* WriteBuffer init */
///* wrinte date to EEPROM */
//if(HAL_I2C_Mem_Write(&hi2c1, ADDR_24LCxx_Write, 10, I2C_MEMADD_SIZE_8BIT,WriteBuffer,1, 0x10) == HAL_OK)
//    printf("\r\n EEPROM 24C02 Write Test OK \r\n");
//else
//    printf("\r\n EEPROM 24C02 Write Test False \r\n");
// 
///* read date from EEPROM */
//HAL_I2C_Mem_Read(&hi2c1, ADDR_24LCxx_Read, 10, I2C_MEMADD_SIZE_8BIT,ReadBuffer,1, 0x10);
//for(i=0; i<256; i++)
//    printf("0x%02X  ",ReadBuffer[i]);

//if(memcmp(WriteBuffer,ReadBuffer,1) == 0 ) /* check date */
//    printf("\r\n EEPROM 24C02 Read Test OK\r\n");
//else
//    printf("\r\n EEPROM 24C02 Read Test False\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
   
//			hmc5983_init();
//	  HAL_RTC_GetTime(&hrtc,&RTC_TimeStruct,RTC_FORMAT_BIN);
//		HAL_RTC_GetDate(&hrtc,&RTC_DateStruct,RTC_FORMAT_BIN);
////			hmc5983_init();
//		 snprintf(buf,9,"%02d:%02d:%02d,",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds);
//GUI_DispStringAt(buf,50,100);
//      hmc5983_init();
//      for(i=1;i<8;i++)
//      {
//      CreateWindow();
//      GUIDEMO_Main();
//        GUI_DispDecAt(i,100,60,2);
        hmc5983_redbuf(&x,&y,&z,i);
        
//      CreateWindow();
     for(i=1;i<8;i++)
    {
    snprintf(buf,20,"%d",hmc5983.x[i]);
    LISTVIEW_SetItemText(hItem, 1, i-1, buf);
        snprintf(buf,20,"%d",hmc5983.y[i]);
    LISTVIEW_SetItemText(hItem, 2, i-1, buf);
        snprintf(buf,20,"%d",hmc5983.z[i]);
    LISTVIEW_SetItemText(hItem, 3, i-1, buf);
         snprintf(buf,20,"%d",hmc5983.a1x[i]);
    LISTVIEW_SetItemText(hItem, 6, i-1, buf);
        snprintf(buf,20,"%d",hmc5983.a1y[i]);
    LISTVIEW_SetItemText(hItem, 7, i-1, buf);
        snprintf(buf,20,"%d",hmc5983.a1z[i]);
    LISTVIEW_SetItemText(hItem, 8, i-1, buf);
                snprintf(buf,20,"%d",hmc5983.dif[i]);
    LISTVIEW_SetItemText(hItem, 9, i-1, buf);
        

    }
//      LCD_ControlCache(LCD_CC_FLUSH);
//                   for(i=1;i<8;i++)
//    {
//    snprintf(buf,20,"%d\n%d\n%d\n",hmc5983.x[i],hmc5983.y[i],hmc5983.z[i]);
//    LISTVIEW_SetItemText(hItem, 1, i-1, buf);
////    snprintf(buf,20,"%d\n%d\n%d\n",hmc5983.a0x[i],hmc5983.a0y[i],hmc5983.a0z[i]);
////    LISTVIEW_SetItemText(hItem, 5, i-1, buf);
//    }
//	  	snprintf(buf,20,"x%4d,y%4d,z%4d,",x,y,z);
//	    GUI_DispStringAt(buf,50,200);
         
//	  }
//	  if(f_open(&MyFile, "buff.csv", FA_OPEN_ALWAYS|FA_WRITE) != FR_OK)  Error_Handler();			
//	  if( f_lseek(&MyFile, f_size(&MyFile))!= FR_OK) Error_Handler();

//	  if(	f_printf(&MyFile, "%d,%d,%d,",x,y,z)== EOF)Error_Handler();
//		if(	f_printf(&MyFile, "%02d:%02d:%02d\n",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds)== EOF)Error_Handler();  
////		if(	f_printf(&MyFile, "20%02d-%02d-%02d\n", RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date)== EOF)Error_Handler(); 
////		 
//    f_sync(&MyFile);		
//		f_close(&MyFile);

//		HAL_Delay(500);

//		HAL_Delay(500);
//					HAL_Delay(100);

				WM_Paint(hWin);	
  GUI_Delay(500);
//GUI_Exit();
               
  }
	
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
//	
//static  void  AppTaskStart (void *p_arg)
//{
//    OS_ERR  err;

//	uint16_t i;
//	uint8_t  led_fx;

//   (void)p_arg;

////    BSP_Init();                                                 /* Initialize BSP functions                             */
//    BSP_Tick_Init();                                            /* Initialize Tick Services.                            */


//#if OS_CFG_STAT_TASK_EN > 0u
//    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
//#endif

//#ifdef CPU_CFG_INT_DIS_MEAS_EN
//    CPU_IntDisMeasMaxCurReset();
//#endif
//	
//#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
//	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
//	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err); 
//#endif		
//	
//								  OSTaskCreate((OS_TCB       *)&App_TaskledTCB,              /* Create the start task                                */
//                 (CPU_CHAR     *)"App led Start",
//                 (OS_TASK_PTR   )led0_task,
//                 (void         *)0u,
//                 (OS_PRIO       )APP_CFG_TASK_led_PRIO,
//                 (CPU_STK      *)&App_TaskledStk[0u],
//                 (CPU_STK_SIZE  )APP_CFG_TASK_led_STK_SIZE / 10u,
//                 (CPU_STK_SIZE  )APP_CFG_TASK_led_STK_SIZE,
//                 (OS_MSG_QTY    )0u,
//                 (OS_TICK       )0u,
//                 (void         *)0u,
//                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR       *)&err);
//								 
////				OSTaskSuspend(OS_PRIO_SELF,&err); 	//挂起start任务					 
//    while (DEF_TRUE) 
//			{ 
//		/* Task body, always written as an infinite loop.       */
//				OSTimeDlyHMSM(0u, 0u, 0u, 1u,
//                      OS_OPT_TIME_HMSM_STRICT,
//                      &err);
//				if(led_fx==1)//
//        {i++;}
//        else{i--;}
//        if(i>950)
//            led_fx=0;
//        if(i<10)
//            led_fx=1;
//				TIM3->CCR1 = i;
//				TIM1->CCR1 = 1000;
//				TIM3->CCR2 = i;

////		GUI_DispStringAt("Hello world!", (LCD_GetXSize()-100)/2, (LCD_GetYSize()-20)/2);
////				LCD_ShowxNum(200,200,i,4,24,0);
//			}
//}

////led0任务函数
//void led0_task(void *p_arg)
//{
//	OS_ERR err;
//	(void)p_arg;
//	uint8_t i;

//	uint32_t ad1,ad2,ad3,ad4,ad5;
//	while(1)
//	{
//		
//		for(i=0,ad1=0,ad2=0,ad3=0,ad4=0,ad5=0;i<40;)
//		{
//		    ad1+=ADCxConvertedValue2[i++];
//				ad2+=ADCxConvertedValue2[i++];
//				ad3+=ADCxConvertedValue2[i++];
//				ad4+=ADCxConvertedValue2[i++];
//				ad5+=ADCxConvertedValue2[i++];
//		
//		
//		}
//		ADCxConvertedValue[0]=ad1/8;
//		ADCxConvertedValue[1]=ad2/8;
//		ADCxConvertedValue[2]=ad3/8;
//		ADCxConvertedValue[3]=ad4/8;
//		ADCxConvertedValue[4]=ad5/8;
//		
//		
//		if(f_open(&MyFile, "adcbuff.txt", FA_OPEN_ALWAYS|FA_WRITE) != FR_OK)
//            {
//              /* 'STM32.TXT' file Open for read Error */
//              Error_Handler();
//            }
//						
//	  if( f_lseek(&MyFile, f_size(&MyFile))!= FR_OK)
//            {
//              /* 'STM32.TXT' file Open for read Error */
//              Error_Handler();
//            }
////		for(i=0;i<5;i++)	
////		{
////			snprintf(adcbuf,5,"%4d,",ADCxConvertedValue[i]);
////			if(  f_write(&MyFile, adcbuf, sizeof(adcbuf), &bw)!= FR_OK)Error_Handler();  
////		}
////			snprintf(buf,9,"%02d:%02d:%02d,",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds); 
////			if(  f_write(&MyFile, buf, 9, &bw)!= FR_OK)Error_Handler();
////			GUI_DispStringAt(buf,30,140);
////		
////			snprintf(buf,14,"20%02d-%02d-%02d\r\n",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date);
////			if(  f_write(&MyFile, buf, 14, &bw)!= FR_OK)Error_Handler();			
////			GUI_DispStringAt(buf,30,180);	
//						
//	OSTimeDlyHMSM(0u, 0u, 0u, 500u,
//                      OS_OPT_TIME_HMSM_STRICT,
//                      &err);
//	}
//}
/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通讯
*	形    参: p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
//static  void  AppObjCreate (void)
//{

//	/* 创建同步信号量 */ 
//   	BSP_OS_SemCreate(&SEM_SYNCH,
//					 0,	
//					 (CPU_CHAR *)"SEM_SYNCH");				 
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
	GUI_DispStringAt("error_handler",100,180);
  while(1) 
  {
			
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
