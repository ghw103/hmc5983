/*********************************************************************
*          Portions COPYRIGHT 2015 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.28 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "R61408_LCD.h"
/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//
#define XSIZE_PHYS  480 // To be adapted to x-screen size
#define YSIZE_PHYS  800 // To be adapted to y-screen size

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
static void LcdWriteReg(U16 Data) {
  // ... TBD by user
	*(__IO uint16_t *) (Bank1_LCD_C) = Data;
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) {
  // ... TBD by user
	*(__IO uint16_t *) (Bank1_LCD_D) = Data;
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    // ... TBD by user
			*(__IO uint16_t *) (Bank1_LCD_D) = *pData++;
  }
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    // ... TBD by user
		*pData++=*(__IO uint16_t *) (Bank1_LCD_D);
  }
}

/*********************************************************************
*
*       _ReadPixel
*/
static U16 _ReadPixel(int LayerIndex) {
//	uint16_t Index;
////	LcdWriteReg(0x2a);   
////	WriteData(x>>8);
////	WriteData(x&0xff);
////	LcdWriteReg(0x2b);   
////	WriteData(y>>8);
////	WriteData(y&0xff);
//	LcdWriteReg(0x2e);
//	Index = *(__IO uint16_t *) (Bank1_LCD_D);
//	Index = *(__IO uint16_t *) (Bank1_LCD_D);
//	return Index;
//  #ifndef WIN32
  U16 aData[3];
  U16 Index;

  GUI_USE_PARA(LayerIndex);
  //
  // Switch to read mode
  //
		LcdWriteReg(0x2E);
  LcdReadDataMultiple(aData, GUI_COUNTOF(aData));
  LcdWriteReg(0x2E);
  //
  // Dummy- and data read
  //
	aData[0]=*(__IO uint16_t *) (Bank1_LCD_D);
	aData[1]=*(__IO uint16_t *) (Bank1_LCD_D);
	aData[2]=*(__IO uint16_t *) (Bank1_LCD_D);
  //
  // Convert to index
  //
  Index = (aData[2] >> 11) | ((aData[1] & 0xfc) << 3) | (aData[1] & 0xf800);
//  #else
//  U16 Index;

//  GUI_USE_PARA(LayerIndex);
//  Index = 0;
//  #endif
  return Index;
}
/*********************************************************************
*
*       _ReadMPixels
*/
static void _ReadMPixels(int LayerIndex, U16 * pBuffer, U32 NumPixels) {

  U16 Data;
  U16 Index;
  int State;

  GUI_USE_PARA(LayerIndex);
  //
  // Switch to read mode
  //
  LcdWriteReg(0x2E);
  //
  // Dummy- read
  //
  Data=*(__IO uint16_t *) (Bank1_LCD_D);
  for (State = 0; NumPixels; NumPixels--, State ^= 1) {
    switch (State) {
    case 0:
      Data=*(__IO uint16_t *) (Bank1_LCD_D);
      Index  =  Data & 0xf800;        // blue
      Index |= (Data & 0x00fc) <<  3; // green
      Data=*(__IO uint16_t *) (Bank1_LCD_D);
      Index |= (Data & 0xf800) >> 11; // red
      break;
    case 1:
      Index  = (Data & 0x00f8) <<  8; // blue
      Data=*(__IO uint16_t *) (Bank1_LCD_D);
      Index |= (Data & 0xfc00) >>  5; // green
      Index |= (Data & 0x00f8) >>  3; // red
      break;
    }
    *pBuffer++ = Index;
  }
	LcdWriteReg(0x2E);
  //
  // Dummy- read
  //
  Data=*(__IO uint16_t *) (Bank1_LCD_D);
  for (State = 0; NumPixels; NumPixels--, State ^= 1) {
    switch (State) {
    case 0:
      Data=*(__IO uint16_t *) (Bank1_LCD_D);
      Index  =  Data & 0xf800;        // blue
      Index |= (Data & 0x00fc) <<  3; // green
      Data=*(__IO uint16_t *) (Bank1_LCD_D);
      Index |= (Data & 0xf800) >> 11; // red
      break;
    case 1:
      Index  = (Data & 0x00f8) <<  8; // blue
      Data=*(__IO uint16_t *) (Bank1_LCD_D);
      Index |= (Data & 0xfc00) >>  5; // green
      Index |= (Data & 0x00f8) >>  3; // red
      break;
    }
    *pBuffer++ = Index;
  }

  return;
}
/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_M565, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  //
  // Orientation
  //
  Config.Orientation = GUI_MIRROR_X | GUI_SWAP_XY;//����
//	Config.NumDummyReads=2;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
  PortAPI.pfWrite16_A0  = LcdWriteReg;
  PortAPI.pfWrite16_A1  = LcdWriteData;
  PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
	LCD_SetDevFunc(0, LCD_DEVFUNC_READPIXEL,   (void(*)(void))_ReadPixel);
	 LCD_SetDevFunc(0, LCD_DEVFUNC_READMPIXELS, (void(*)(void))_ReadMPixels);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
		
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    // ...
			
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/
