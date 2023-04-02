//
// PfcMatrix.h
//
// (C) MobiFlight Project 2022
//

#pragma once

namespace PfcMatrix
{
    void Add(int dataPin, int csPin, int clkPin,
             uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4,
             uint8_t digit5, uint8_t digit6, uint8_t digit7, uint8_t digit8,
             uint8_t digit9, uint8_t digit10);
    void Clear();
    void PowerSave(bool state);
    void OnSetDisplay();
    void OnSet();
    void update();
}

// PfcMatrix.h
