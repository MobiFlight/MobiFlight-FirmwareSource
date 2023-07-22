#include "MFCustomDevice.h"
#include "commandmessenger.h"
#include "allocateMem.h"

/* **********************************************************************************
    Within the connector up to 6 pins, a device name and a config string can be defined
    These informations are stored in the EEPROM like for the other devices.
    While reading the config from the EEPROM this function is called.
    It is the first function which will be called for the custom device.
    If it fits into the memory buffer, the constructor for the customer device
    will be called
********************************************************************************** */

MFCustomDevice::MFCustomDevice(char *customPins, char *customType, char *configuration)
{
    _initialized = true;
    /* **********************************************************************************
        Do something which is required to setup your custom device
    ********************************************************************************** */

    char *params, *p = NULL;
    /* **********************************************************************************
        Read the pins from the string stored in the eeprom
        This is just an example how to process the string.
        Number of Pins could be more or less, it's depending what you have defined
        in the device.json file
    ********************************************************************************** */
    params = strtok_r(customPins, "|", &p);
    _pin1  = atoi(params);
    params = strtok_r(NULL, "|", &p);
    _pin2  = atoi(params);
    params = strtok_r(NULL, "|", &p);
    _pin3  = atoi(params);

    /* **********************************************************************************
        Read the configuration parameters from the string stored in the eeprom
        This is just an example how to process the init string. Do NOT use
        "," or ";" as delimiter for multiple parameters but e.g. "|"
        For most customer devices it is not required.
        In this case just delete the following
    ********************************************************************************** */
    uint8_t  Parameter1, Parameter2;
    char    *Parameter3;
    uint16_t Parameter4;

    params     = strtok_r(configuration, "|", &p);
    Parameter1 = atoi(params);
    params     = strtok_r(NULL, "|", &p);
    Parameter2 = atoi(params);
    params     = strtok_r(NULL, "|", &p);
    Parameter3 = params;
    params     = strtok_r(NULL, "|", &p);
    Parameter4 = atoi(params);
    /* ******************************************************************************* */

    /* **********************************************************************************
        Next call the constructor of your custom device
        adapt it to the needs of your constructor
    ********************************************************************************** */
    if (!FitInMemory(sizeof(MyCustomDevice))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("Custom Device does not fit in Memory"));
        return;
    }
    _mydevice = new (allocateMemory(sizeof(MyCustomDevice))) MyCustomDevice(Parameter1, Parameter2);
    _mydevice->attach(Parameter3, Parameter4);
}

/* **********************************************************************************
    The custom devives gets unregistered if a new config gets uploaded.
    Keep it as it is, mostly nothing must be changed.
    It gets called from CustomerDevice::Clear()
********************************************************************************** */
void MFCustomDevice::detach()
{
    _initialized = false;
    _mydevice->detach();
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
void MFCustomDevice::set(uint8_t messageID, char *setPoint)
{
    if (!_initialized) return;

    _mydevice->set(messageID, setPoint);
}
