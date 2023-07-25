#include "MFCustomDevice.h"
#include "commandmessenger.h"
#include "allocateMem.h"
#include "MyCustomDevice.h"
#include "MFEEPROM.h"
extern MFEEPROM MFeeprom;

/* **********************************************************************************
    The custom device pins, type and configuration is stored in the EEPROM
    While loading the config the adresses in the EEPROM are transferred to the constructor
    Within the constructor you have to copy the EEPROM content to a buffer
    and evaluate him. The buffer is used for all 3 types (pins, type configuration),
    so do it step by step.
    The max size of the buffer is defined here. It must be the size of the
    expected max length of these strings.

    E.g. 6 pins are required, each pin could have two characters (two digits),
    each pins are delimited by "|" and the string is NULL terminated.
    -> (6 * 2) + 5 + 1 = 18 bytes is the maximum.
    The custom type is "MyCustomDevice", which means 14 characters plus NULL = 15
    The configuration is "myConfig", which means 8 characters plus NULL = 9
    The maximum characters to be expected is 18, so MEMLEN_STRING_BUFFER has to be at least 18
********************************************************************************** */
#define MEMLEN_STRING_BUFFER 40

// reads a string from EEPROM at given address which is '.' terminated and saves it to the buffer
bool MFCustomDevice::getStringFromEEPROM(uint16_t addreeprom, char *buffer)
{
    char    temp    = 0;
    uint8_t counter = 0;
    do {
        temp              = MFeeprom.read_byte((addreeprom)++); // read the first character
        buffer[counter++] = temp;                               // save character and locate next buffer position
        if (counter >= MEMLEN_STRING_BUFFER) {                  // nameBuffer will be exceeded
            return false;                                       // abort copying to buffer
        }
    } while (temp != '.');                                      // reads until limiter '.' and locates the next free buffer position
    buffer[counter - 1] = 0x00;                                 // replace '.' by NULL, terminates the string
    return true;
}

/* **********************************************************************************
    Within the connector pins, a device name and a config string can be defined
    These informations are stored in the EEPROM like for the other devices.
    While reading the config from the EEPROM this function is called.
    It is the first function which will be called for the custom device.
    If it fits into the memory buffer, the constructor for the customer device
    will be called
********************************************************************************** */

MFCustomDevice::MFCustomDevice(uint16_t adrPin, uint16_t adrType, uint16_t adrConfig)
{
    _initialized = true;
    /* **********************************************************************************
        Do something which is required to setup your custom device
    ********************************************************************************** */

    char *params, *p = NULL;
    char  parameter[MEMLEN_STRING_BUFFER];
    /* **********************************************************************************************
        read the pins from the EEPROM, copy them into a buffer and split them up die single pins
    ********************************************************************************************** */
    getStringFromEEPROM(adrPin, parameter);
    params = strtok_r(parameter, "|", &p);
    _pin1  = atoi(params);
    params = strtok_r(NULL, "|", &p);
    _pin2  = atoi(params);
    params = strtok_r(NULL, "|", &p);
    _pin3  = atoi(params);

    /* **********************************************************************************
        read the Type from the EEPROM, copy it into a buffer and evaluate it
        it's only required if your custom device handles multiple devices with
        different contructors
        the string get's NOT stored as this would need a lot of RAM, instead a variable
        is used to store the type
    ********************************************************************************** */
    getStringFromEEPROM(adrType, parameter);
    if (strcmp(parameter, MY_CUSTOM_TYPE_1) == 0)
        _customType = MY_CUSTOM_DEVICE_1;
    if (strcmp(parameter, MY_CUSTOM_TYPE_2) == 0)
        _customType = MY_CUSTOM_DEVICE_2;
    /* ******************************************************************************* */

    /* **********************************************************************************
        read the configuration from the EEPROM, copy it into a buffer and evaluate it.
        This is just an example how to process the init string. Do NOT use
        "," or ";" as delimiter for multiple parameters but e.g. "|"
        For most customer devices it is not required.
        In this case just delete the following
    ********************************************************************************** */
    getStringFromEEPROM(adrConfig, parameter);
    uint16_t Parameter1;
    char    *Parameter2;
    params     = strtok_r(parameter, "|", &p);
    Parameter1 = atoi(params);
    params     = strtok_r(NULL, "|", &p);
    Parameter2 = params;
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
    _mydevice = new (allocateMemory(sizeof(MyCustomDevice))) MyCustomDevice(_pin1, _pin2);
    _mydevice->attach(Parameter1, Parameter2);
    /* ******************************************************************************* */
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

/* **********************************************************************************
    This function reports back the type of the custom device.
    Only custom devices which match this type can be added within the UI
********************************************************************************** */
char *MFCustomDevice::getType()
{
    if (_customType == MY_CUSTOM_DEVICE_1)
        return MY_CUSTOM_TYPE_1;
    if (_customType == MY_CUSTOM_DEVICE_2)
        return MY_CUSTOM_TYPE_2;
}