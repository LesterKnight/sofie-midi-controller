#pragma once
#include <Arduino.h>

class TonexController {
private:
  static uint8_t currentPatch;
  static uint8_t previousPatch;
  

  static constexpr uint8_t PATCH_MIN = 0;
  static constexpr uint8_t PATCH_MAX = 127;

  /*
  static constexpr uint8_t BOOST_VOL = 0;
  static constexpr bool DELAY = false;
  static constexpr uint8_t DELAY_PARAM1 = 0;
  static constexpr bool MODULATION = false;
  static constexpr uint8_t MODULATION_PARAM1 = 0;
  static constexpr bool REVERVB = false;
  static constexpr uint8_t REVERB_PARAM1 = 0;
  static constexpr bool NOISEGATE = false;
  static constexpr uint8_t REVERB_PARAM1 = 0;
  static constexpr uint8_t NOISEGATE_PARAM1 = 0;
  */

public:
  static int BOOST;
  static uint8_t getPatch() {
    return currentPatch;
  }

  static bool patchHasChanged() {
    if (currentPatch != previousPatch) {
      previousPatch = currentPatch;
      return true;
    }
    return false;
  }

  static void nextPatch() {
    if (currentPatch < PATCH_MAX) {
      currentPatch++;
    }
  }

  static void prevPatch() {
    if (currentPatch > PATCH_MIN) {
      currentPatch--;
    }
  }

  static void sendPatch() {
    Serial.write(0xC0);              // Program Change, canal 1
    Serial.write(currentPatch);
  }

  static void getPatchName(char* buffer) {
    uint8_t bank = currentPatch / 3;
    uint8_t slot = (currentPatch % 3) + 1;

    char letter = 'A' + bank;
    if (letter > 'Z') {
      letter = '?';
    }

    buffer[0] = letter;
    buffer[1] = '0' + slot;
    buffer[2] = '\0'; // terminador nulo
  }

  static void loadPatch(char* buffer) {
    
  }

};


uint8_t TonexController::currentPatch = 0;
uint8_t TonexController::previousPatch = 0;
int TonexController::BOOST = 0;