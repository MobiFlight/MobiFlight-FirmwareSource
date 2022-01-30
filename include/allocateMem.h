#ifndef allocateMEM_h
#define allocateMEM_h

#include <new>

extern char deviceBuffer[];

char * allocateMemory(uint8_t size);
void ClearMemory();
uint16_t GetAvailableMemory();
bool FitInMemory(uint8_t size);

#endif
