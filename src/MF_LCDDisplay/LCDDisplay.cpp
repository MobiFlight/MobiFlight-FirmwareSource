#include <Arduino.h>
#include "MFLCDDisplay.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include "LCDDisplay.h"
#include "MFBoards.h"
namespace LCDDisplay
{
MFLCDDisplay *lcd_I2C[MAX_MFLCD_I2C];
uint8_t lcd_12cRegistered = 0;


void Add(uint8_t address = 0x24, uint8_t cols = 16, uint8_t lines = 2)
{
  if (lcd_12cRegistered == MAX_MFLCD_I2C)
    return;

  if (!FitInMemory(sizeof(MFLCDDisplay)))
	{
		// Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
		return;
	}
  lcd_I2C[lcd_12cRegistered] = new (allocateMemory(sizeof(MFLCDDisplay))) MFLCDDisplay;
  lcd_I2C[lcd_12cRegistered]->attach(address, cols, lines);
  lcd_12cRegistered++;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
}

void Clear()
{
  for (int i = 0; i != lcd_12cRegistered; i++)
  {
    lcd_I2C[i]->detach();
  }
  lcd_12cRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Cleared lcdDisplays"));
#endif
}

void OnSet()
{
  int address = cmdMessenger.readInt16Arg();
  char *output = cmdMessenger.readStringArg();
  lcd_I2C[address]->display(output);
  setLastCommandMillis();
}
}   // end of namespace LCDDisplay
