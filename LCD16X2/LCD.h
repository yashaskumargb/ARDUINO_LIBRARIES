#ifndef LCD_H
#define LCD_H


#include "ArduinoMega.h"

/*FLAG POSITIONS*/
#define INS_ENTRY_MODE_SET_SH 0
#define INS_ENTRY_MODE_SET_ID 1
#define INS_DISPLAY_CTRL_B   0
#define INS_DISPLAY_CTRL_C   1
#define INS_DISPLAY_CTRL_D   2
#define INS_CURSOR_CTRL_RL   2
#define INS_CURSOR_CTRL_SC   3
#define INS_FUNCTION_SET_F   2
#define INS_FUNCTION_SET_N   3
#define INS_FUNCTION_SET_DL  4

#define INS_FUNCTION_SET_F_SIZE_5X8  0
#define INS_FUNCTION_SET_F_SIZE_5X11 1

#define INS_ENTRY_MODE_SET_ID_INCREMENT 1
#define INS_ENTRY_MODE_SET_ID_DECREMENT 0
typedef struct _LCD_s
{
  //dptr=DDR reg
  volatile char *dptr_dataLines;
  //ctrl lines PORT
  volatile char *dptr_RSLine;
  volatile char *dptr_ENLine;
  //ctrl Lines pin number
  uint8_t RS_Pin,EN_Pin;
  uint8_t ClearDisplay=0x01;
  uint8_t ReturnHome=0x02;
  uint8_t EntryModeSet=0x04;
  uint8_t DisplayCtrl=0x08;
  uint8_t CursorCtrl=0x10;
  uint8_t FunctionSet=0x20;
  uint8_t setDDRAMAddr=0x80;
  uint8_t writeData2DDRAM=0x00;//RS=1;R/w=GND
  //set CGram address and Read DDRAM DAta is not requried
  uint8_t rows;
}LCD;




 void LCD_Init(LCD *dev,uint16_t dataLines_t,uint16_t ctrlLine_RS_PORTName_t,uint8_t ctrlLine_RS_PIN_t,uint16_t ctrlLine_EN_PORTName,uint8_t ctrlLine_EN_PIN_t,uint8_t fontSize,uint8_t rows_t,uint8_t dataLinesCnt);

 void LCD_clearDisplay(const LCD *dev);

 void LCD_blinkON(LCD *dev);
 void LCD_blinkOFF(LCD *dev);

 void LCD_displayChar(const LCD *dev,char symbol);
 void LCD_printNumber(LCD *dev,uint16_t num);
 void LCD_printString(LCD *dev,const char *stringData);

 void LCD_setCursor(const LCD *dev,uint8_t row_t,uint8_t col_t);
 void LCD_resetCursor(LCD *dev);

 void LCD_cursorON(LCD *dev);
 void LCD_cursorOFF(LCD *dev);

 void LCD_DisplayOn(LCD *dev);
 void LCD_DisplayOff(LCD *dev);

 void LCD_scrollDisplayLeft(LCD *dev);
 void LCD_scrollDisplayRight(LCD *dev);

 void LCD_leftToRight(LCD *dev);
 void LCD_rightToLeft(LCD *dev);

 void LCD_autoScroll(LCD *dev);
 void LCD_noAutoScroll(LCD *dev);





#endif