#include "LCD.h"

static void pin_init(LCD *dev,uint16_t dataLines,uint16_t ctrlLine_RS_PORTName,uint8_t ctrlLine_RS_PIN,uint16_t ctrlLine_EN_PORTName,uint8_t ctrlLine_EN_PIN);
static void write_dataIR(const LCD *dev,uint8_t val);
static void write_dataDR(const LCD *dev,uint8_t val);
static void enableAction(const LCD *dev);


 static void pin_init(LCD *dev,uint16_t dataLines,uint16_t ctrlLine_RS_PORTName,uint8_t ctrlLine_RS_PIN,uint16_t ctrlLine_EN_PORTName,uint8_t ctrlLine_EN_PIN) 
{
  /*Assign teh address to pointers the LCD obj*/
  dev->dptr_dataLines=(volatile char *)dataLines;
  dev->dptr_ENLine=(volatile char *)ctrlLine_EN_PORTName;
  dev->dptr_RSLine=(volatile char *)ctrlLine_RS_PORTName;

  /*Assign pin of of specific port to which EN pin and RS pin are connected*/
  dev->RS_Pin=ctrlLine_RS_PIN;
  dev->EN_Pin=ctrlLine_EN_PIN;

  /*Configure direction of dataLine as output */
  *(dev->dptr_dataLines)=0xFF;
  
  /*Configure direction of EN pin as output */
  *(dev->dptr_ENLine)|=(0x01<<(dev->EN_Pin));
  
  /*Configure direction of RS pin as output */
  *(dev->dptr_RSLine)|=(0x01<<(dev->RS_Pin));
}
 static void write_dataIR(const LCD *dev,uint8_t val)
{
  *((dev->dptr_RSLine)+1)&=~(0x01<<(dev->RS_Pin));//RS=0
  *((dev->dptr_dataLines)+1)=val;

}
 static void write_dataDR(const LCD *dev,uint8_t val)
{
  *((dev->dptr_RSLine)+1)|=(0x01<<(dev->RS_Pin));//RS=1
  *((dev->dptr_dataLines)+1)=val;

}
 static void enableAction(const LCD *dev)
{
  *((dev->dptr_ENLine)+1)|=(0x01<<dev->EN_Pin);
  delayMicroseconds(1);
  *((dev->dptr_ENLine)+1)&=~(0x01<<dev->EN_Pin);
  delayMicroseconds(1);
}


void LCD_Init(LCD *dev,uint16_t dataLines_t,uint16_t ctrlLine_RS_PORTName_t,uint8_t ctrlLine_RS_PIN_t,uint16_t ctrlLine_EN_PORTName,uint8_t ctrlLine_EN_PIN_t,uint8_t fontSize,uint8_t rows_t,uint8_t dataLinesCnt)
{

  pin_init(dev, dataLines_t, ctrlLine_RS_PORTName_t, ctrlLine_RS_PIN_t, ctrlLine_EN_PORTName, ctrlLine_EN_PIN_t);
/*Configure Function Set Instruction-DL(Datalines):8lines/4lins*/
if(dataLinesCnt==8)
  (dev->FunctionSet)|=(0x01<<INS_FUNCTION_SET_DL);
else if (dataLinesCnt==4)
  (dev->FunctionSet)&=~(0x01<<INS_FUNCTION_SET_DL);

/*Configure Function Set Instruction-N(NumOfRows):2rows/1rows*/
if(rows_t==2)
  dev->FunctionSet|=(0x01<<INS_FUNCTION_SET_N);
else if (rows_t==1)
  dev->FunctionSet&=~(0x01<<INS_FUNCTION_SET_N);

/*Configure Function Set Instruction-F(fontSize):5X11/5X8*/
if((fontSize)==INS_FUNCTION_SET_F_SIZE_5X11)
dev->FunctionSet|=(0x01<<INS_FUNCTION_SET_F);
else if ((fontSize)==INS_FUNCTION_SET_F_SIZE_5X8)
dev->FunctionSet&=~(0x01<<INS_FUNCTION_SET_F);



/*default Values*/
/*Configure DisplayCtrl Instruction-B(Blink):ON/OFF*/
dev->DisplayCtrl|=(0x01<<INS_DISPLAY_CTRL_B);
/*Configure DisplayCtrl Instruction-C(Cursor):ON/OFF*/
dev->DisplayCtrl|=(0x01<<INS_DISPLAY_CTRL_C);
/*Configure DisplayCtrl Instruction-D(Display):ON/OFF*/
dev->DisplayCtrl|=(0x01<<INS_DISPLAY_CTRL_D);

/*Configure EntryMode Set Instruction-S(Shift):ON/OFF*/
dev->EntryModeSet|=(0x01<<INS_ENTRY_MODE_SET_SH);
/*Configure EntryMode Set Instruction-I/D(Inc/Dec):ON/OFF*/
dev->EntryModeSet|=(0x01<<INS_ENTRY_MODE_SET_ID);

/*write values in structure to LCD ctrler*/
  write_dataIR(dev,dev->FunctionSet);//DL:8bits;No of Line:2;Font size:5X8
  enableAction(dev);
  delay(1);

  write_dataIR(dev,dev->DisplayCtrl);//Dispaly ON;Blink CUrsor/Curson On
  enableAction(dev);
  delay(1);

  write_dataIR(dev,dev->ClearDisplay);//Clear Dispaly
  enableAction(dev);
  delay(1);

  write_dataIR(dev,dev->ReturnHome);//Return to Home
  enableAction(dev);
  delay(1);
}

void LCD_clearDisplay(const LCD *dev)
{
  write_dataIR(dev,dev->ClearDisplay);
  enableAction(dev);
}

void LCD_blinkON(LCD *dev)
{
  dev->DisplayCtrl|=(0x01<<INS_DISPLAY_CTRL_B);
  write_dataIR(dev,dev->DisplayCtrl);
  enableAction(dev);
}

void LCD_blinkOFF(LCD *dev)
{
  dev->DisplayCtrl&=~(0x01<<INS_DISPLAY_CTRL_B);
  write_dataIR(dev,dev->DisplayCtrl);
  enableAction(dev);
}

void LCD_displayChar(const LCD *dev,char symbol)
{ 
  write_dataDR(dev,(uint8_t)symbol);
  enableAction(dev);
}



void LCD_printString(LCD *dev,const char *stringData)
{
  while(*stringData!='\0')
  {
    LCD_displayChar(dev,*stringData);
    //delay(500);
    stringData++;
  }
}

void LCD_printNumber(LCD *dev,uint16_t num)
{
  char numStr[7]="";
  snprintf(&numStr[0],sizeof(numStr),"%u",num);
 LCD_printString(dev,&numStr[0]);
}
void LCD_setCursor(const LCD *dev,uint8_t row_t,uint8_t col_t)
{
  if(row_t ==1)
   write_dataIR(dev,(dev->setDDRAMAddr)|(col_t-1));
  else if(row_t == 2)
   write_dataIR(dev,((dev->setDDRAMAddr)|(64+col_t-1)));
  enableAction(dev);
}

void LCD_resetCursor(LCD *dev)
{
  write_dataIR(dev,dev->ReturnHome);
  enableAction(dev);
}

void LCD_cursorON(LCD *dev)
{
  dev->DisplayCtrl|=(0x01<<INS_DISPLAY_CTRL_C);
  write_dataIR(dev,dev->DisplayCtrl);
  enableAction(dev);
}
void LCD_cursorOFF(LCD *dev)
{
  dev->DisplayCtrl&=~(0x01<<INS_DISPLAY_CTRL_C);
  write_dataIR(dev,dev->DisplayCtrl);
  enableAction(dev);
}

void LCD_DisplayOn(LCD *dev)
{
  dev->DisplayCtrl|=(0x01<<INS_DISPLAY_CTRL_D);
  write_dataIR(dev,dev->DisplayCtrl);
  enableAction(dev);
}
void LCD_DisplayOff(LCD *dev)
{
  dev->DisplayCtrl&=~(0x01<<INS_DISPLAY_CTRL_D);
  write_dataIR(dev,dev->DisplayCtrl);
  enableAction(dev);
}

void LCD_scrollDisplayLeft(LCD *dev)
{
  dev->CursorCtrl|=(0x01<<INS_CURSOR_CTRL_SC);
  dev->CursorCtrl&=~(0x01<<INS_CURSOR_CTRL_RL);
  write_dataIR(dev,dev->CursorCtrl);
  enableAction(dev);
}
void LCD_scrollDisplayRight(LCD *dev)
{
  dev->CursorCtrl|=(0x01<<INS_CURSOR_CTRL_SC);
  dev->CursorCtrl|=(0x01<<INS_CURSOR_CTRL_RL);
  write_dataIR(dev,dev->CursorCtrl);
  enableAction(dev);
}

void LCD_leftToRight(LCD *dev) {
  dev->EntryModeSet|=(0x01<<INS_ENTRY_MODE_SET_ID);
  write_dataIR(dev,dev->EntryModeSet);
  enableAction(dev);
}
void LCD_rightToLeft(LCD *dev) {
  dev->EntryModeSet&=~(0x01<<INS_ENTRY_MODE_SET_ID);
  write_dataIR(dev,dev->EntryModeSet);
  enableAction(dev);
}

void LCD_autoScroll(LCD *dev)
{
  dev->EntryModeSet|=(0x01<<INS_ENTRY_MODE_SET_SH);
  write_dataIR(dev,dev->EntryModeSet);
  enableAction(dev);
}
void LCD_noAutoScroll(LCD *dev)
{
    dev->EntryModeSet&=~(0x01<<INS_ENTRY_MODE_SET_SH);
    write_dataIR(dev,dev->EntryModeSet);
    enableAction(dev);
}
