//
// stowManager.cpp
//
// (C) MobiFlight Project 2022
// 

#include "stowManager.h"
#include <new>

// Returns the free size in bytes
uint16_t StowManager::
getFreeSize(void)
{
    return (uint16_t)(bufTop-tail);
}

// Returns the occupied size in bytes
uint16_t StowManager::
getUsedSize(void)
{
    return (uint16_t)(tail-buf);
}

// Adds item to end of buffer, returns false if full
uint8_t* StowManager::
add(uint8_t reqdSize, uint8_t typeTag)
{   
    uint8_t *res = NULL;
    uint8_t *newTail;
    
    newTail = tail + reqdSize + 2;
    if(newTail < bufTop) {
        *tail++ = reqdSize + 2;
        *tail++ = typeTag;
        res = tail;
        tail = newTail;
        i_count++;
    }
    return res; 
}    

// Resets current item to first one
//inline 
void StowManager::
reset(void)
{   
    curr = buf;
    i_curr = 0;
}    

// Empties buffer (resets pointers)
//inline 
void StowManager::
wipe(void)
{
    tail = buf;
    curr = buf;
    i_count = 0;
    i_curr = 0;
}    

// helper function
uint8_t* StowManager::
_next(uint8_t **ptr)
{
    uint8_t* res = NULL;
    if(*ptr < tail) {
        res = *ptr;   // returns the complete data with size & type tag 
        *ptr += *res;
        i_curr++;
    }
    return res;
}    

// returns the type code of next item, or 0xFF if none (does NOT affect current item pointer)//inline 
uint8_t StowManager::
getTypeOfNext(void)
{
    return ((curr < tail) ? *(curr + 1) : 0xFF);
}    

// returns the next item with typecode <type> (advances item pointer)
uint8_t* StowManager::
getNext(uint8_t type)
{
    uint8_t* res;
    do {
        res = _next(&curr);
    } while( (res!= NULL) && (type != TypeALL) && (*(res+1) != type)); 
    return (res != NULL ? res+2 : NULL);
}    

// returns the n-th item with typecode <type> from the start (does not affect current item pointer)
uint8_t* StowManager::
getNth(t_index nth, uint8_t type)
{
    uint8_t* res;
    uint8_t* ptr = 0;
    t_index  cnt = 0;
    do {
        res = _next(&ptr);
        if((type != 0xFF)&&(*(res+1) == type)) cnt++;
    } while( (res!= NULL) && /* (*(res+1) != type) && */ (cnt <= nth)); 
    return (res != NULL ? res+2 : NULL);
}    

// stowManager.cpp

