#pragma once

#include "Arduino.h"
#include "MyCustomDevice.h"
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

class GNC255
{
public:
    GNC255(uint8_t clk, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset);
    void begin();
    void attach();
    void detach();
    void set(uint8_t messageID, char *setPoint);

private:
    U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI *_oledDisplay;
    bool                                 _initialised;
    uint8_t                              _clk, _data, _cs, _dc, _reset;

    void _update();
    void _stop();
    void updateActiveFreq(char *frequency);
    void updateStandbyFreq(char *frequency);
    void updateActiveLabel(char *frequency);
    void updateStandbyLabel(char *frequency);
    void _updateFrequency(char *label, uint8_t x, uint8_t y);
    void _updateLabel(char *label, uint8_t x, uint8_t y);
    void _staticLabels(char *label, uint8_t x, uint8_t y);
};