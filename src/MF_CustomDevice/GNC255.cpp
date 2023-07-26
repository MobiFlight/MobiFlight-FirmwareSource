#include "GNC255.h"
#include <U8g2lib.h>
#include "allocateMem.h"
#include "commandmessenger.h"

GNC255::GNC255(uint8_t clk, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset)
{
    _clk   = clk;
    _data  = data;
    _cs    = cs;
    _dc    = dc;
    _reset = reset;
}

void GNC255::attach()
{
    /* **********************************************************************************
        Next call the constructor of your custom device
        adapt it to the needs of your constructor
    ********************************************************************************** */
    if (!FitInMemory(sizeof(U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("Custom Device does not fit in Memory"));
        return;
    }

    _oledDisplay = new (allocateMemory(sizeof(U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI))) U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI(U8G2_R0, _clk, _data, _cs, _dc, _reset);
    begin();
}

void GNC255::begin()
{
    _oledDisplay->begin();
    _oledDisplay->clearBuffer();
    _update();
    _oledDisplay->sendBuffer();
}

void GNC255::_update()
{
    // This problem applies only to full buffer mode
    updateActiveFreq("121.500");
    updateActiveLabel("EMERGENCY");
    updateStandbyFreq("121.700");
    updateStandbyLabel("KPWK GND");
}

void GNC255::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void GNC255::_stop()
{
    _oledDisplay->clearBuffer();
    _oledDisplay->sendBuffer();
}

void GNC255::set(uint8_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do:
    ********************************************************************************** */
    const char *data = setPoint;
    uint16_t    output;

    // do something according your messageID
    switch (messageID) {
    case 0:
        _stop();
        break;

    // set Active Frequency
    case 1:
        updateActiveFreq(setPoint);
        break;

    // set Standby Frequency
    case 2:
        updateStandbyFreq(setPoint);
        break;
    case 3:
        updateActiveLabel(setPoint);
        break;
    case 4:
        updateStandbyFreq(setPoint);
        break;
    default:
        break;
    }
}

void GNC255::updateActiveFreq(char *frequency)
{
    _updateFrequency(frequency, 10, 32);
}

void GNC255::updateStandbyFreq(char *frequency)
{
    _updateFrequency(frequency, 140, 32);
}

void GNC255::updateActiveLabel(char *frequency)
{
    _updateLabel(frequency, 36, 45);
}

void GNC255::updateStandbyLabel(char *frequency)
{
    _updateLabel(frequency, 165, 45);
}

void GNC255::_updateFrequency(char *frequency, uint8_t x, uint8_t y)
{
    _oledDisplay->setDrawColor(0);
    _oledDisplay->drawBox(x + 5, y - 50, 90, 50);
    _oledDisplay->setDrawColor(1);
    _oledDisplay->setFontMode(0);
    _oledDisplay->setCursor(x, y);
    _oledDisplay->setFont(u8g2_font_logisoso22_tn);
    _oledDisplay->print(frequency);
    _staticLabels("COM", 105, 18);
    _staticLabels("ACT", 0, 32);
    _staticLabels("STB", 130, 32);
    _oledDisplay->sendBuffer();
}

void GNC255::_updateLabel(char *frequency, uint8_t x, uint8_t y)
{
    _oledDisplay->setDrawColor(0);
    _oledDisplay->drawBox(x - 20, y - 12, 85, 15);
    _oledDisplay->setDrawColor(1);
    _oledDisplay->setFontMode(0);
    _oledDisplay->setCursor(x, y);
    _oledDisplay->setFont(u8g2_font_profont12_mr);
    _oledDisplay->print(frequency);
    _oledDisplay->sendBuffer();
}

void GNC255::_staticLabels(char *label, uint8_t x, uint8_t y)
{
    _oledDisplay->setDrawColor(0);
    _oledDisplay->drawBox(x, y - 7, 15, 7);
    _oledDisplay->setDrawColor(1);
    _oledDisplay->setFontMode(0);
    _oledDisplay->setCursor(x, y);
    _oledDisplay->setFont(u8g2_font_profont10_mr);
    _oledDisplay->print(label);
    //_oledDisplay->sendBuffer();
}