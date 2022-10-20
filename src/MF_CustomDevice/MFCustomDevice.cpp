#include "MFCustomDevice.h"

CustomDeviceEvent MFCustomDevice::_handler = NULL;

/* **********************************************************************************
    The constructor is just an example, change it to your demands of your custom device
    Adapt also the number of parameters you need here and in the MFCustomDevice.h file
    Also adapt it in CustomDevice.cpp.
    It gets called from CustomerDevice::Add()
********************************************************************************** */
MFCustomDevice::MFCustomDevice(uint8_t init1, uint8_t init2, uint8_t init3, const char *initParameter, uint16_t init4)
{
    _initialized = true;
    /* **********************************************************************************
        Do something which is required to setup your custom device
    ********************************************************************************** */
}

/* **********************************************************************************
    The custom devives gets unregistered if a new config gets uploaded.
    Keep it as it is, mostly nothing must be changed.
    It gets called from CustomerDevice::Clear()
********************************************************************************** */
void MFCustomDevice::detach()
{
    _initialized = false;
}

/* **********************************************************************************
    Within in loop() the update() function is called regularly
    Within the loop() you can define a time delay where this function gets called
    or as fast as possible. See comments in loop().
    It is only needed if you have to update your custom device without getting
    new values from the connector.
    Otherwise just make a return; in the calling function.
    It gets called from CustomerDevice::update()
********************************************************************************** */
void MFCustomDevice::update()
{
    if (!_initialized) return;
    /* **********************************************************************************
        Do something if required
    ********************************************************************************** */
}

/* **********************************************************************************
    If an output for the custom device is defined in the connector,
    this function gets called when a new value is available.
    It gets called from CustomerDevice::OnSet()
********************************************************************************** */
void MFCustomDevice::set(char *setPoint)
{
    if (!_initialized) return;

    /* **********************************************************************************
        You can process the string here at this point to get multiple parameters or
        just transfer it to your custom devive and process it there.
        In this example two int16_t values and one string is expected from the connector
        These three informations have the delimiter "," which must be
        within the string defined in the connector
    ********************************************************************************** */
    char *params, *p = NULL;

    params            = strtok_r(setPoint, ",", &p);
    int16_t newValue1 = atoi(params);

    params            = strtok_r(NULL, ",", &p);
    int16_t newValue2 = atoi(params);

    params = strtok_r(NULL, ",", &p);

    /* **********************************************************************************
        Do something with the parameters
    ********************************************************************************** */
   newValue1 = newValue2;
   newValue2 = newValue1;
}

void MFCustomDevice::attachHandler(CustomDeviceEvent newHandler)
{
    _handler = newHandler;
}
