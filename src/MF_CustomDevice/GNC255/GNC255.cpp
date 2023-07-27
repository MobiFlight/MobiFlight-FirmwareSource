#include "GNC255.h"
#include <U8g2lib.h>
#include "allocateMem.h"
#include "commandmessenger.h"

/*
struct Layout {
    Label Value;
    Label ValueLabel;
    Label Station;
    Label ModeComLabel;
    Label ModeNavLabel;
};
*/
Layout ComLayout = {
    {u8g2_font_logisoso22_tn, 22, {10, 32}},
    {u8g2_font_profont10_mr, 10, {0, 32}},
    {u8g2_font_profont12_mr, 13, {18, 45}},
    {u8g2_font_profont12_mr, 12, {107, 18}},
    {u8g2_font_profont12_mr, 12, {120, 18}},
};

Position OffsetActive = {
    0,
    0};

Position OffsetStandby = {
    140,
    0};

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

    _oledDisplay = new (allocateMemory(sizeof(U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI))) U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI(U8G2_R0, _clk, _data, _cs, _dc);
    //_oledDisplay = new (allocateMemory(sizeof(U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI))) U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI(U8G2_R0, 53, _dc, _reset);
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
    updateActiveFreq("188.888");
    updateActiveLabel("MobiFlight");
    updateStandbyFreq("188.888");
    updateStandbyLabel("rocks!");
    setMode(true);
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

void GNC255::set(uint8_t messageID, const char *data)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do:
    ********************************************************************************** */
    // do something according your messageID
    switch (messageID) {
    case 0:
        _stop();
        break;

    // set Active Frequency
    case 1:
        updateActiveFreq(data);
        break;
    // set Standby Frequency
    case 2:
        updateStandbyFreq(data);
        break;
    case 3:
        updateActiveLabel(data);
        break;
    case 4:
        updateStandbyLabel(data);
        break;
    case 5:
        setMode(strcmp(data, "0") == 0);
        break;
    default:
        break;
    }
    _oledDisplay->sendBuffer();
}

void GNC255::setMode(bool isCom)
{
    if (isCom) {
        _renderLabel("   ", ComLayout.ModeNavLabel, OffsetActive, true);
        _renderLabel("COM", ComLayout.ModeComLabel, OffsetActive, true);
    }

    else {
        _renderLabel("   ", ComLayout.ModeComLabel, OffsetActive, true);
        _renderLabel("NAV", ComLayout.ModeNavLabel, OffsetActive, true);
    }
}

void GNC255::updateActiveFreq(const char *frequency)
{
    _renderLabel(frequency, ComLayout.Value, OffsetActive);
    _renderLabel("ACT", ComLayout.ValueLabel, OffsetActive);
}

void GNC255::updateStandbyFreq(const char *frequency)
{
    _renderLabel(frequency, ComLayout.Value, OffsetStandby);
    _renderLabel("STB", ComLayout.ValueLabel, OffsetStandby);
}

void GNC255::updateActiveLabel(const char *frequency)
{
    _renderLabel(frequency, ComLayout.Station, OffsetActive);
}

void GNC255::updateStandbyLabel(const char *frequency)
{
    _renderLabel(frequency, ComLayout.Station, OffsetStandby);
}

void GNC255::_renderLabel(const char *text, Label label, Position offset, bool update)
{
    _oledDisplay->setFont(label.Font);
    u8g2_int_t w = _oledDisplay->getStrWidth(text);
    u8g2_int_t h = label.FontSize;
    _oledDisplay->setDrawColor(0);
    _oledDisplay->drawBox(offset.x + label.Pos.x, offset.y + label.Pos.y - h, w, h);
    _oledDisplay->setDrawColor(1);
    _oledDisplay->setFontMode(0);
    _oledDisplay->setCursor(offset.x + label.Pos.x, offset.y + label.Pos.y);

    _oledDisplay->print(text);
    if (update) _oledDisplay->sendBuffer();
}