#include <Arduino.h>
#include "CustomDevice.h"
#include "MFCustomDevice.h"
#include "commandmessenger.h"
#include "MFBoards.h"
#include "allocateMem.h"

/* **********************************************************************************
        Normally nothing has to be changed in this file
        It handles one or multiple custom devices
********************************************************************************** */
namespace CustomDevice
{
    uint8_t         CustomDeviceRegistered = 0;
    MFCustomDevice *customDevice[MAX_CUSTOM_DEVICES];

    void Add(uint16_t adrPin, uint16_t adrType, uint16_t adrConfig)
    {
        if (CustomDeviceRegistered == MAX_CUSTOM_DEVICES)
            return;
        if (!FitInMemory(sizeof(MFCustomDevice))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Custom Device does not fit in Memory"));
            return;
        }
        customDevice[CustomDeviceRegistered] = new (allocateMemory(sizeof(MFCustomDevice))) MFCustomDevice(adrPin, adrType, adrConfig);
        CustomDeviceRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added Stepper Setpoint"));
#endif
    }

    /* **********************************************************************************
        All custom devives gets unregistered if a new config gets uploaded.
        The Clear() function from every registerd custom device will be called.
        Keep it as it is, mostly nothing must be changed
    ********************************************************************************** */
    void Clear()
    {
        for (int i = 0; i != CustomDeviceRegistered; i++) {
            customDevice[i]->detach();
        }
        CustomDeviceRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Cleared Stepper Setpoint"));
#endif
    }

    /* **********************************************************************************
        Within in loop() the update() function is called regularly
        Within the loop() you can define a time delay where this function gets called
        or as fast as possible. See comments in loop()
        The update() function from every registerd custom device will be called.
        It is only needed if you have to update your custom device without getting
        new values from the connector. Otherwise just make a return;
    ********************************************************************************** */
    void update()
    {
        for (int i = 0; i != CustomDeviceRegistered; i++) {
            customDevice[i]->update();
        }
    }

    /* **********************************************************************************
        If an output for the custom device is defined in the connector,
        this function gets called when a new value is available.
        In this case the connector sends a messageID followed by a string which is not longer
        than 90 Byte (!!check the length of the string!!) limited by the commandMessenger.
        The messageID is used to mark the value which has changed. This reduced the serial
        communication as not all values has to be send in one (big) string (like for the LCD)
        The OnSet() function from every registerd custom device will be called.
    ********************************************************************************** */
    void OnSet()
    {
        int device = cmdMessenger.readInt16Arg();       // get the device number
        if (device >= CustomDeviceRegistered)           // and do nothing if this device is not registered
            return;
        int   messageID = cmdMessenger.readInt16Arg();  // get the messageID number
        char *output    = cmdMessenger.readStringArg(); // get the pointer to the new raw string
        cmdMessenger.unescape(output);                  // and unescape the string if escape characters are used
        customDevice[device]->set(messageID, output);   // send the string to your custom device

        setLastCommandMillis();
    }
} // end of namespace