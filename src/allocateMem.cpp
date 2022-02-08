#include <Arduino.h>
#include "MFBoards.h"
#include "mobiflight.h"
#include "allocateMem.h"
#include "CommandMessenger.h"

char deviceBuffer[MF_MAX_DEVICEMEM] = {0};
uint16_t nextPointer = 0;

char * allocateMemory(uint8_t size)
{
    uint16_t actualPointer = nextPointer;
    nextPointer = actualPointer + size;
    if (nextPointer >= MF_MAX_DEVICEMEM)
    {
        cmdMessenger.sendCmd(kStatus,F("DeviceBuffer Overflow!"));
        return nullptr;
    }
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmdStart(kStatus);
    cmdMessenger.sendCmdArg(F("BufferUsage"));
    cmdMessenger.sendCmdArg(nextPointer);
    cmdMessenger.sendCmdEnd();
#endif
    return &deviceBuffer[actualPointer];
}

void ClearMemory() {
    nextPointer = 0;
}

uint16_t GetAvailableMemory() {
    return MF_MAX_DEVICEMEM - nextPointer;
}

bool FitInMemory(uint8_t size) {
    if (nextPointer + size > MF_MAX_DEVICEMEM)
        return false;
    return true;
}
