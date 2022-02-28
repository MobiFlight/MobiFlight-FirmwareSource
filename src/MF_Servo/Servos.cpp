#include <Arduino.h>
#include "MFServo.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include "Servos.h"
#include "MFBoards.h"

namespace Servos
{
MFServo *servos[MAX_MFSERVOS];
uint8_t servosRegistered = 0;


void Add(int pin)
{
  if (servosRegistered == MAX_MFSERVOS)
    return;

  if (!FitInMemory(sizeof(MFServo)))
	{
		// Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("Servo does not fit in Memory!"));
		return;
	}
  servos[servosRegistered] = new (allocateMemory(sizeof(MFServo))) MFServo;
  servos[servosRegistered]->attach(pin, true);
  servosRegistered++;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Added servos"));
#endif
}

void Clear()
{
  for (int i = 0; i != servosRegistered; i++)
  {
    servos[i]->detach();
  }
  servosRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Cleared servos"));
#endif
}

void OnSet()
{
  int servo = cmdMessenger.readInt16Arg();
  int newValue = cmdMessenger.readInt16Arg();
  if (servo >= servosRegistered)
    return;
  servos[servo]->moveTo(newValue);
  setLastCommandMillis();
}

void update()
{
  for (int i = 0; i != servosRegistered; i++)
  {
    servos[i]->update();
  }
}
}   // end of namespace Servo
