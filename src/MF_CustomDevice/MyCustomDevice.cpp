#include "MyCustomDevice.h"

MyCustomDevice::MyCustomDevice()
{
}

MyCustomDevice::MyCustomDevice(uint8_t Pin1, uint8_t Pin2)
{
    _pin1 = Pin1;
    _pin2 = Pin2;
}
void MyCustomDevice::begin()
{
}

void MyCustomDevice::attach(uint16_t Pin3, char *init)
{
    _pin3 = Pin3;
    begin();
}
void MyCustomDevice::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void MyCustomDevice::set(uint8_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do:
    ********************************************************************************** */
    int32_t  data = atoi(setPoint);
    uint16_t output;

    // do something according your messageID
    switch (messageID) {
    case 1:
        output = (uint16_t)data;
        data   = output;
        break;
    case 2:
        /* code */
        break;
    case 3:
        /* code */
        break;
    default:
        break;
    }
}