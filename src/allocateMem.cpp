//
// allocatemem.cpp
//
// (C) MobiFlight Project 2022
//

#include "allocateMem.h"
#include "commandmessenger.h"

alignas(max_align_t) static uint8_t deviceBuffer[MF_MAX_DEVICEMEM] = {0};
static size_t nextPointer                                          = 0;

static size_t alignUp(size_t value, size_t alignment)
{
    if (alignment == 0) return value;

    size_t remainder = value % alignment;
    if (remainder == 0) return value;

    return value + (alignment - remainder);
}

void *allocateMemory(size_t size, size_t alignment)
{
    if (size == 0) return nullptr;
    if (alignment == 0) alignment = 1;

    size_t actualPointer = alignUp(nextPointer, alignment);

    if (actualPointer > MF_MAX_DEVICEMEM || size > MF_MAX_DEVICEMEM - actualPointer) {
        cmdMessenger.sendCmd(kStatus, F("DeviceBuffer Overflow!"));
        return nullptr;
    }

    size_t newPointer = actualPointer + size;
    nextPointer       = newPointer;
#ifdef DEBUG2CMDMESSENGER
    cmdMessenger.sendCmdStart(kDebug);
    cmdMessenger.sendCmdArg(F("Bytes added"));
    cmdMessenger.sendCmdArg(size);
    cmdMessenger.sendCmdEnd();
    cmdMessenger.sendCmdStart(kDebug);
    cmdMessenger.sendCmdArg(F("BufferUsage"));
    cmdMessenger.sendCmdArg(nextPointer);
    cmdMessenger.sendCmdEnd();
#endif
    return &deviceBuffer[actualPointer];
}

void ClearMemory()
{
    nextPointer = 0;
}

size_t GetAvailableMemory()
{
    return MF_MAX_DEVICEMEM - nextPointer;
}
// allocatemem.cpp
