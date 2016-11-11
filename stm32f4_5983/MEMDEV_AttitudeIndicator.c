/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2004  SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : MEMDEV_AttitudeIndicator.c
Purpose     : Shows use of high resolution rotation for memory devices
Requirements: WindowManager - ( )
              MemoryDevices - (x)
              AntiAliasing  - (x)
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/

#include <stddef.h>
#include "GUI.h"

#if GUI_WINSUPPORT
  #include "WM.h"
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define COLOR_SKY    0xd8aa5d
#define COLOR_GROUND 0x2d93c7
#define AA_FACTOR 6
#define SPEED_MAX_PITCH  (500 * 25)
#define SPEED_MAX_ROLL  (1200 * 25)
#define BUTTON_UP    0
#define BUTTON_DOWN  1
#define BUTTON_LEFT  2
#define BUTTON_RIGHT 3

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  GUI_MEMDEV_Handle hMemMask;          // Mask device for punch operation
  GUI_MEMDEV_Handle hMemScale;         // Content of complete scale
  GUI_MEMDEV_Handle hMemWork;          // Destination device
  GUI_POINT         aPointSrc[3];      // Source points for roll indicator triangle
  GUI_POINT         aPointDst[3];      // Destination points for roll indicator
  GUI_POINT         aPointSrcCL[4];    // Initial coordinates of clearing polygon left
  GUI_POINT         aPointSrcCR[4];    // Initial coordinates of clearing polygon left
  GUI_POINT         aPointDstCL[4];    // Initial coordinates of clearing polygon right
  GUI_POINT         aPointDstCR[4];    // Initial coordinates of clearing polygon right
  GUI_RECT          RectUD;            // Up/down touch area
  GUI_RECT          RectLR;            // Left/right touch area
  I32               Pitch;
  I32               Roll;
  int               xSizeDisplay;
  int               ySizeDisplay;
  int               r;
  int               d;
  int               xm;
  int               ym;
} ANIM_CONTEXT;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/****************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _CreateMask
*/
static GUI_MEMDEV_Handle _CreateMask(int xPos, int yPos, int xSize, int ySize, int r) {
  GUI_MEMDEV_Handle hMem;
  int               xm;
  int               ym;

  xm = xPos + xSize / 2;
  ym = yPos + ySize / 2;
  hMem = GUI_MEMDEV_CreateFixed(xPos, yPos, xSize, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_8, GUICC_8);
  GUI_MEMDEV_Select(hMem);
  GUI_SetColor(GUI_WHITE);
  GUI_AA_FillCircle(xm, ym, r);
  GUI_MEMDEV_Select(0);
  return hMem;
}

/*********************************************************************
*
*       _CreateScale
*/
static GUI_MEMDEV_Handle _CreateScale(int xm, int r) {
  GUI_MEMDEV_Handle hMemScale;
  GUI_RECT          Rect;
  char              acBuffer[] = "90\0";
  int               x0;
  int               x1;
  int               yPosLine;
  int               xEndLine;
  int               xSizeText;
  int               ySizeText;
  int               xPosScale;
  int               xSizeScale;
  int               ySizeScale;
  int               ym;
  int               i;
  
  xEndLine   = 0;
  xPosScale  = xm - r;
  xSizeScale = r * 2 + 1;
  ySizeScale = (r * 2 + 1) * 3;
  ym         = ySizeScale / 2;
  hMemScale  = GUI_MEMDEV_CreateFixed(xPosScale, 0, xSizeScale, ySizeScale, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_8888);
  GUI_MEMDEV_Select(hMemScale);
  //
  // Sky
  //
  GUI_SetColor(COLOR_SKY);
  GUI_FillRect(xPosScale, 0, xPosScale + xSizeScale - 1, ySizeScale / 2 - 1);
  //
  // Ground
  //
  GUI_SetColor(COLOR_GROUND);
  GUI_FillRect(xPosScale, ySizeScale / 2, xPosScale + xSizeScale - 1, ySizeScale - 1);
  //
  // Scale
  //
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextMode(GUI_TM_TRANS);
  xSizeText = GUI_GetStringDistX("XXX");
  ySizeText = GUI_GetFontSizeY();
  GUI_DrawHLine(ym + 0, xPosScale, xPosScale + xSizeScale - 1);
  GUI_DrawHLine(ym + 1, xPosScale, xPosScale + xSizeScale - 1);
  GUI_DrawVLine(xm, 0, ySizeScale - 1);
  for (i = 1; i <= 9 * 4; i++) {
    yPosLine = (i * r) / 12;
    switch (i % 4) {
    case 0:
      xEndLine = r / 3;
      Rect.x0 = xPosScale + xSizeScale / 2 - xEndLine - xSizeText;
      Rect.y0 = ym - yPosLine - ySizeText / 2;
      Rect.x1 = Rect.x0 + xSizeText - 1;
      Rect.y1 = Rect.y0 + ySizeText - 1;
      acBuffer[0] = '0' + i / 4;
      GUI_DispStringInRect(acBuffer, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
      GUI_MoveRect(&Rect, xEndLine * 2 + xSizeText, 0);
      GUI_DispStringInRect(acBuffer, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
      GUI_MoveRect(&Rect, 0, yPosLine * 2);
      GUI_DispStringInRect(acBuffer, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
      GUI_MoveRect(&Rect, -xEndLine * 2 - xSizeText, 0);
      GUI_DispStringInRect(acBuffer, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
      break;
    case 1:
    case 3:
      xEndLine = r / 12;
      break;
    case 2:
      xEndLine = r / 6;
      break;
    }
    x0 = xPosScale + xSizeScale / 2 - xEndLine;
    x1 = xPosScale + xSizeScale / 2 + xEndLine;
    GUI_DrawHLine(ym + yPosLine + 0, x0, x1);
    GUI_DrawHLine(ym + yPosLine + 1, x0, x1);
    GUI_DrawHLine(ym - yPosLine - 0, x0, x1);
    GUI_DrawHLine(ym - yPosLine - 1, x0, x1);
  }
  GUI_MEMDEV_Select(0);
  return hMemScale;
}

/*********************************************************************
*
*       _DrawButton
*/
static void _DrawButton(ANIM_CONTEXT * pContext, int Pressed, int Index) {
  GUI_MEMDEV_Handle   hMem;
  GUI_MEMDEV_Handle   hMemOld;
  const GUI_RECT    * pClipOld;
  GUI_RECT            rClip;
  int                 xm;
  int                 ym;
  int                 xd;
  int                 yd;
  int                 s;
  int                 x;
  int                 y;

  xm = ym = xd = yd = 0;
  GUI_SetPenSize(2);
  GUI_SetBkColor(GUI_BLACK);
  GUI_SetColor(GUI_WHITE);
  //
  // Calculate and set clip rect
  //
  rClip = ((Index == BUTTON_LEFT) || (Index == BUTTON_RIGHT)) ? pContext->RectLR : pContext->RectUD;
  x     = rClip.x0 + ((rClip.x1 - rClip.x0) + 1) / 2;
  y     = rClip.y0 + ((rClip.y1 - rClip.y0) + 1) / 2;
  switch (Index) {
  case BUTTON_UP:
    rClip.y1 = y;
    break;
  case BUTTON_DOWN:
    rClip.y0 = y;
    break;
  case BUTTON_LEFT:
    rClip.x1 = x;
    break;
  case BUTTON_RIGHT:
    rClip.x0 = x;
    break;
  }
  pClipOld = GUI_SetClipRect(&rClip);
  //
  // Create & select memory device
  //
  hMem    = GUI_MEMDEV_CreateEx(rClip.x0, rClip.y0, rClip.x1, rClip.y1, GUI_MEMDEV_NOTRANS);
  hMemOld = GUI_MEMDEV_Select(hMem);
  GUI_Clear();
  //
  // Calculate values for arrow & draw background and arrow
  //
  switch (Index) {
  case BUTTON_UP:
  case BUTTON_DOWN:
    ym = (pContext->RectLR.y1 + pContext->RectUD.y0) / 2;
    yd = (pContext->RectLR.y1 - pContext->RectUD.y0) / 8;
    xm = (pContext->RectUD.x1 + pContext->RectUD.x0) / 2;
    xd = (pContext->RectUD.x1 - pContext->RectUD.x0) / 8;
    if (Pressed) {
      GUI_AA_FillRoundedRectEx(&pContext->RectUD, 5);
      GUI_SetColor(GUI_BLACK);
    } else {
      GUI_AA_DrawRoundedRectEx(&pContext->RectUD, 5);
      GUI_DrawHLine(ym, pContext->RectUD.x0, pContext->RectUD.x1);
    }
    break;
  case BUTTON_LEFT:
  case BUTTON_RIGHT:
    ym = (pContext->RectLR.y1 + pContext->RectLR.y0) / 2;
    yd = (pContext->RectLR.y1 - pContext->RectLR.y0) / 8;
    xm = (pContext->RectLR.x1 + pContext->RectLR.x0) / 2;
    xd = (pContext->RectLR.x1 - pContext->RectLR.x0) / 8;
    if (Pressed) {
      GUI_AA_FillRoundedRectEx(&pContext->RectLR, 5);
      GUI_SetColor(GUI_BLACK);
    } else {
      GUI_AA_DrawRoundedRectEx(&pContext->RectLR, 5);
      GUI_DrawVLine(xm, pContext->RectLR.y0, pContext->RectLR.y1);
    }
    break;
  }
  if ((Index == BUTTON_LEFT) || (Index == BUTTON_RIGHT)) {
    s = (Index == BUTTON_LEFT) ? -1 : +1;
    GUI_AA_DrawLine(xm + 3 * xd * s, ym         ,
                    xm + 1 * xd * s, ym + 2 * yd);
    GUI_AA_DrawLine(xm + 3 * xd * s, ym         ,
                    xm + 1 * xd * s, ym - 2 * yd);
  } else {
    s = (Index == BUTTON_UP) ? -1 : +1;
    GUI_AA_DrawLine(xm         , ym + 3 * yd * s,
                    xm + 2 * xd, ym + 1 * yd * s);
    GUI_AA_DrawLine(xm         , ym + 3 * yd * s,
                    xm - 2 * xd, ym + 1 * yd * s);
  }
  //
  // Draw and delete memory device
  //
  GUI_MEMDEV_Select(hMemOld);
  GUI_MEMDEV_Write(hMem);
  GUI_MEMDEV_Delete(hMem);
  GUI_SetClipRect(pClipOld);
}

/*********************************************************************
*
*       _InitAnimContext
*/
static int _InitAnimContext(ANIM_CONTEXT * pContext) {
  int xPosWork;
  int yPosWork;
  int xSizeWork;
  int ySizeWork;
  int d;
  I32 SinHQ;
  I32 CosHQ;

  pContext->xSizeDisplay = LCD_GetXSize();
  pContext->ySizeDisplay = LCD_GetYSize();
  pContext->xm           = pContext->xSizeDisplay / 2;
  pContext->ym           = pContext->ySizeDisplay / 2;
  pContext->r            = pContext->ySizeDisplay / 3;
  pContext->d            = pContext->r / 14;
  xPosWork               = pContext->xm - pContext->r * 6 / 7;
  xSizeWork              = (pContext->r * 6 / 7) * 2 + 1;
  yPosWork               = pContext->ym - pContext->r;
  ySizeWork              = pContext->r * 2 + 1;
  pContext->hMemMask     = _CreateMask(xPosWork, yPosWork, xSizeWork, ySizeWork, pContext->r);
  if (pContext->hMemMask == 0) {
    return 1;
  }
  pContext->hMemWork = GUI_MEMDEV_CreateFixed(xPosWork, yPosWork, xSizeWork, ySizeWork, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_8888);
  if (pContext->hMemWork == 0) {
    GUI_MEMDEV_Delete(pContext->hMemMask);
    return 1;
  }
  pContext->hMemScale = _CreateScale(pContext->xm, pContext->r);
  if (pContext->hMemScale == 0) {
    GUI_MEMDEV_Delete(pContext->hMemWork);
    GUI_MEMDEV_Delete(pContext->hMemMask);
    return 1;
  }
  pContext->aPointSrc[0].x = 0;
  pContext->aPointSrc[0].y = (pContext->r + pContext->d * 2) * AA_FACTOR;
  SinHQ = GUI__SinHQ(60000);
  CosHQ = GUI__CosHQ(60000);
  //
  // Roll indicator
  //
  pContext->aPointSrc[1].x = pContext->aPointSrc[0].x - (((CosHQ * pContext->d * 2) >> 16) * AA_FACTOR);
  pContext->aPointSrc[1].y = pContext->aPointSrc[0].y - (((SinHQ * pContext->d * 2) >> 16) * AA_FACTOR);
  pContext->aPointSrc[2].x = pContext->aPointSrc[0].x + (((CosHQ * pContext->d * 2) >> 16) * AA_FACTOR);
  pContext->aPointSrc[2].y = pContext->aPointSrc[1].y;
  //
  // Polygon for clearing left side of indicator
  //
  pContext->aPointSrcCL[0].x = pContext->aPointSrc[0].x;
  pContext->aPointSrcCL[0].y = pContext->aPointSrc[0].y;
  pContext->aPointSrcCL[1].x = pContext->aPointSrc[1].x;
  pContext->aPointSrcCL[1].y = pContext->aPointSrc[1].y - 4;
  pContext->aPointSrcCL[2].x = pContext->aPointSrcCL[1].x - AA_FACTOR * 4;
  pContext->aPointSrcCL[2].y = pContext->aPointSrcCL[1].y;
  pContext->aPointSrcCL[3].x = pContext->aPointSrcCL[0].x - AA_FACTOR * 4;
  pContext->aPointSrcCL[3].y = pContext->aPointSrcCL[0].y;
  //
  // Polygon for clearing right side of indicator
  //
  pContext->aPointSrcCR[0].x = pContext->aPointSrc[0].x;
  pContext->aPointSrcCR[0].y = pContext->aPointSrc[0].y;
  pContext->aPointSrcCR[1].x = pContext->aPointSrc[2].x;
  pContext->aPointSrcCR[1].y = pContext->aPointSrc[2].y - 4;
  pContext->aPointSrcCR[2].x = pContext->aPointSrcCR[1].x + AA_FACTOR * 4;
  pContext->aPointSrcCR[2].y = pContext->aPointSrcCR[1].y;
  pContext->aPointSrcCR[3].x = pContext->aPointSrcCR[0].x + AA_FACTOR * 4;
  pContext->aPointSrcCR[3].y = pContext->aPointSrcCR[0].y;
  //
  // Surrounding rectangle
  //
  GUI_AA_DrawRoundedRect(0, 0, pContext->xSizeDisplay - 1, pContext->ySizeDisplay - 1, 5);
  //
  // Calculate button areas
  //
  d                   = pContext->xSizeDisplay / 16;
  pContext->RectLR.x0 = d * 0 + 10;
  pContext->RectLR.x1 = pContext->RectLR.x0 + d * 3;
  pContext->RectUD.x1 = pContext->xSizeDisplay - pContext->RectLR.x0 - 1;
  pContext->RectUD.x0 = pContext->RectUD.x1 - (pContext->RectLR.x1 - pContext->RectLR.x0);
  d                   = pContext->ySizeDisplay / 16;
  pContext->RectLR.y0 = d * 4;
  pContext->RectLR.y1 = pContext->RectLR.y0 + d * 4;
  pContext->RectUD.y1 = pContext->RectLR.y1;
  pContext->RectUD.y0 = pContext->RectLR.y0;
  return 0;
}

/*********************************************************************
*
*       _ShowAttitudeIndicator
*/
static void _ShowAttitudeIndicator(ANIM_CONTEXT * pContext) {
  float a;
  int   dx;
  int   dy;
  int   mx;
  int   my;
  int   mxCross;
  int   myCross;
  int   xSizeScale;
  int   ySizeScale;
  int   xPosWork;
  int   yPosWork;
  int   xSizeWork;
  int   ySizeWork;
  I32   sinHQ;
  I32   cosHQ;

  //
  // Rotate memory device in HR mode
  //
  xSizeScale = GUI_MEMDEV_GetXSize(pContext->hMemScale);
  ySizeScale = GUI_MEMDEV_GetYSize(pContext->hMemScale);
  xPosWork   = GUI_MEMDEV_GetXPos(pContext->hMemWork);
  yPosWork   = GUI_MEMDEV_GetYPos(pContext->hMemWork);
  xSizeWork  = GUI_MEMDEV_GetXSize(pContext->hMemWork);
  ySizeWork  = GUI_MEMDEV_GetYSize(pContext->hMemWork);
  sinHQ      = GUI__SinHQ(pContext->Roll);
  cosHQ      = GUI__CosHQ(pContext->Roll);
  dx         = ((((ySizeScale * 4) * (I32)pContext->Pitch) / 90000) * sinHQ) >> 16;
  dy         = ((((ySizeScale * 4) * (I32)pContext->Pitch) / 90000) * cosHQ) >> 16;
  mx         = -(xSizeScale - xSizeWork) * 4;
  my         = -(ySizeScale - ySizeWork) * 4;
  mxCross    = xPosWork + xSizeWork / 2;
  myCross    = yPosWork + ySizeWork / 2;
  GUI_MEMDEV_RotateHQHR(pContext->hMemScale, pContext->hMemWork, mx + dx, my + dy, pContext->Roll, 1000);
  //
  // Punch out area defined in mask device
  //
  GUI_SetBkColor(GUI_BLACK);
  GUI_MEMDEV_PunchOutDevice(pContext->hMemWork, pContext->hMemMask);
  //
  // Draw black cross in center of device
  //
  GUI_MEMDEV_Select(pContext->hMemWork);
  GUI_SetColor(GUI_BLACK);
  GUI_SetPenSize(3);
  GUI_DrawLine(mxCross - 10, myCross, mxCross + 10, myCross);
  GUI_DrawLine(mxCross, myCross - 10, mxCross, myCross + 10);
  //
  // Show the result
  //
  GUI_MEMDEV_Select(0);
  GUI_MEMDEV_Write(pContext->hMemWork);
  //
  // Enable hires antialiasing
  //
  GUI_AA_EnableHiRes();
  GUI_AA_SetFactor(AA_FACTOR);
  a = (pContext->Roll * 3.1415926f) / 180000;
  //
  // Clear right, left and top side of roll indicator
  //
  GUI_SetColor(GUI_BLACK);
  GUI_RotatePolygon(pContext->aPointDstCL, pContext->aPointSrcCL, GUI_COUNTOF(pContext->aPointDstCL), a);
  GUI_AA_FillPolygon(pContext->aPointDstCL, GUI_COUNTOF(pContext->aPointDstCL), pContext->xSizeDisplay / 2 * AA_FACTOR, pContext->ySizeDisplay / 2 * AA_FACTOR);
  GUI_RotatePolygon(pContext->aPointDstCR, pContext->aPointSrcCR, GUI_COUNTOF(pContext->aPointDstCR), a);
  GUI_AA_FillPolygon(pContext->aPointDstCR, GUI_COUNTOF(pContext->aPointDstCR), pContext->xSizeDisplay / 2 * AA_FACTOR, pContext->ySizeDisplay / 2 * AA_FACTOR);
  mx = pContext->xSizeDisplay / 2 * AA_FACTOR;
  my = pContext->ySizeDisplay / 2 * AA_FACTOR;
  GUI_SetColor(GUI_BLACK);
  GUI_SetPenSize(2);
  GUI_AA_DrawLine(mx + pContext->aPointDstCL[1].x, my + pContext->aPointDstCL[1].y, mx + pContext->aPointDstCR[1].x, my + pContext->aPointDstCR[1].y);
  //
  // Draw roll indicator
  //
  GUI_SetColor(GUI_WHITE);
  GUI_RotatePolygon(pContext->aPointDst, pContext->aPointSrc, GUI_COUNTOF(pContext->aPointDst), (pContext->Roll * 3.1415926f) / 180000);
  GUI_AA_FillPolygon(pContext->aPointDst, GUI_COUNTOF(pContext->aPointDst), pContext->xSizeDisplay / 2 * AA_FACTOR, pContext->ySizeDisplay / 2 * AA_FACTOR);
}

/*********************************************************************
*
*       _DrawScale
*/
static void _DrawScale(ANIM_CONTEXT * pAnim) {
  int i;
  int a;
  int xm;
  int ym;
  int rm;
  int x0;
  int y0;
  int x1;
  int x2;
  int y1;
  int r0;
  int r1;
  int s;
  I32 SinHQ;
  I32 CosHQ;

  GUI_AA_EnableHiRes();
  GUI_AA_SetFactor(AA_FACTOR);
  //
  // Draw markers
  //
  GUI_SetPenSize(3);
  r0 = pAnim->r + pAnim->d * 2 + 2;
  r1 = pAnim->r + pAnim->d * 4;
  for (i = 10; i <= 60; i += 10) {
    a = (270 + i) * 1000;
    SinHQ = GUI__SinHQ(a);
    CosHQ = GUI__CosHQ(a);
    y0    = pAnim->ym - ((SinHQ * r0) >> 16);
    y1    = pAnim->ym - ((SinHQ * r1) >> 16);
    x0    = pAnim->xm + ((CosHQ * r0) >> 16);
    x1    = pAnim->xm + ((CosHQ * r1) >> 16);
    GUI_AA_DrawLine(x0 * AA_FACTOR, y0 * AA_FACTOR, x1 * AA_FACTOR, y1 * AA_FACTOR);
    x0 = pAnim->xm - ((CosHQ * r0) >> 16);
    x1 = pAnim->xm - ((CosHQ * r1) >> 16);
    GUI_AA_DrawLine(x0 * AA_FACTOR, y0 * AA_FACTOR, x1 * AA_FACTOR, y1 * AA_FACTOR);
  }
  //
  // Draw center
  //
  GUI_SetPenSize(2);
  SinHQ = GUI__SinHQ(60000);
  CosHQ = GUI__CosHQ(60000);
  s     = (pAnim->d * 3) / 2;
  x0    = pAnim->xm;
  y0    = pAnim->ym + r0;
  y1    = y0 + ((SinHQ * s) >> 16);
  x1    = x0 + ((CosHQ * s) >> 16);
  x2    = x0 - ((CosHQ * s) >> 16);
  GUI_AA_DrawLine(x0 * AA_FACTOR, y0 * AA_FACTOR, x1 * AA_FACTOR, y1 * AA_FACTOR);
  GUI_AA_DrawLine(x0 * AA_FACTOR, y0 * AA_FACTOR, x2 * AA_FACTOR, y1 * AA_FACTOR);
  GUI_AA_DrawLine(x1 * AA_FACTOR, y1 * AA_FACTOR, x2 * AA_FACTOR, y1 * AA_FACTOR);
  GUI_AA_DisableHiRes();
  //
  // Draw text
  //
  GUI_SetPenSize(4);
  GUI_SetTextMode(GUI_TM_TRANS);
  rm = r1 + pAnim->d * 3;
  for (i = 30; i <= 60; i += 30) {
    a = (270 + i) * 1000;
    SinHQ = GUI__SinHQ(a);
    CosHQ = GUI__CosHQ(a);
    ym    = pAnim->ym - ((SinHQ * rm) >> 16);
    xm    = pAnim->xm + ((CosHQ * rm) >> 16);
    GUI_GotoXY(xm, ym);
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
    GUI_DispDecMin(i);
    xm = pAnim->xm - ((CosHQ * rm) >> 16);
    GUI_GotoXY(xm, ym);
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
    GUI_DispDecMin(i);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void MainTask(void);
void MainTask(void) {
  GUI_PID_STATE State;
  ANIM_CONTEXT  Context;
  I32           SpeedPitch;
  I32           SpeedRoll;
  int           Key;
  int           Step;
  int           IndexDraw;
  int           t0;
  int           tNow;
  int           tUsed;
  int           tNext;
  int           Pressed;
  int           PressedOld;

  IndexDraw = 0;
  GUI_Init();
  GUI_SetFont(GUI_FONT_20_ASCII);
  if (_InitAnimContext(&Context)) {
    return;
  }
  //
  // Draw button areas (unpressed) and scale
  //
//  _DrawButton(&Context, 0, BUTTON_UP);
//  _DrawButton(&Context, 0, BUTTON_DOWN);
//  _DrawButton(&Context, 0, BUTTON_LEFT);
//  _DrawButton(&Context, 0, BUTTON_RIGHT);
  _DrawScale(&Context);

  GUI_SetTextAlign(GUI_TA_VCENTER);
  GUI_SetColor(GUI_WHITE);
  #if GUI_WINSUPPORT
    WM_SetWindowPos(WM_HBKWIN, 0, 0, Context.xSizeDisplay, Context.ySizeDisplay);
  #endif
  Step          = 50;
  Context.Pitch = 0;
  Context.Roll  = 0;
  SpeedPitch    =  750; // Millidegrees / s
  SpeedRoll     = 1250; // Millidegrees / s
  PressedOld    = 0;
  t0            = GUI_GetTime();
  do {
    //
    // Draw attitude indicator
    //
    tNext = t0 + 40;
    GUI_MULTIBUF_Begin();
    _ShowAttitudeIndicator(&Context);
    GUI_MULTIBUF_End();
    tNow = GUI_GetTime();
    if (tNext > tNow) {
      GUI_X_Delay(tNext - tNow);
    }
    tUsed = GUI_GetTime() - t0;
    //
    // Manage keyboard
    //
   
    //
    // Change pitch and roll in dependence of speed
    //
    Context.Pitch += 1000;
//    Context.Roll  += 200;
    if (Context.Pitch < -60000) {
      Context.Pitch = 0;
     
    }
    if (Context.Pitch > +60000) {
      Context.Pitch = 0;
   
    }
    if (Context.Roll < -60000) {
      Context.Roll = 0;
   
    }
    if (Context.Roll > 60000) {
      Context.Roll = 0;
    
    }
    t0 = tNow;
  } while (1);
}

/*************************** End of file ****************************/