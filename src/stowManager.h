//
// stowManager.h
//
// (C) MobiFlight Project 2022
// 

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <new>

// A class that manages the stowage of BLOBs in an assigned buffer
class StowManager
{
    //public typedef uint8_t      t_index;
    private:
        typedef uint8_t      t_index;
        
        uint8_t *buf;
        uint8_t *bufTop;
        uint8_t *tail;
        uint8_t *curr;
        
        t_index i_curr;
        t_index i_count;

        uint8_t* _next(uint8_t **ptr);

    public:
        static const uint8_t   TypeALL = 0xFF;

        StowManager(uint8_t *buffer, uint16_t maxSize)
        : buf(buffer), bufTop(buffer+maxSize), tail(buffer), curr(buffer), i_count(0)
        {}

        uint16_t getFreeSize(void);     // Returns the free size in bytes
        uint16_t getUsedSize(void);     // Returns the occupied size in bytes

        uint8_t* add(uint8_t reqdSize, uint8_t typeTag);  // Adds item to end of buffer, returns null if full
        
        void reset(void);           // Resets current item to first one
        void wipe(void);            // Empties buffer (resets pointers)
        
        t_index  getCount(void) { return i_count; }

        uint8_t  getTypeOfNext(void);               // returns the type code of next item, or 0xFF if none (does NOT affect current item pointer)
        uint8_t* getNext(uint8_t type = TypeALL);   // returns the next item, with typecode <type> if specified (advances current item pointer)
        
        uint8_t* getNth(t_index nth, uint8_t type = TypeALL);    // returns the n-th item (base 0) with typecode <type> from the start (does NOT affect current item pointer)


        // THIS METHOD IS STRICTLY APPLICATION-SPECIFIC
        // It requires <T> objects to have two methods, T::getSize(), T::getType(), to be passed 
        // as arguments to the ordinary "add()" 
        // Beside reserving space, it performs a "placement new" of the object of type <T>.
        // Please beware that the argument is a _pointer to_ an object pointer:
        // since an argument referencing the templated type is required in order to set the 
        // method signature, we may as well pass a reference to a pointer that will carry 
        // the return value.
        template <typename T> T* AddItem(T** itemPtr);
};

template <typename T> T* StowManager::AddItem(T** itemPtr)
{
    // Since itemPtr argument is required to set signature anyway, 
    // we take advantage of it to carry the return value 
    uint8_t *in;
    in = add(sizeof(T), T::getType());
    if(in != NULL) {
        new ((void *)in) T;
        // param init done later outside, by specialized functions
    }
    if(*itemPtr != NULL) *itemPtr = (T*)in;
    return (T*)in;
}


// stowManager.h

