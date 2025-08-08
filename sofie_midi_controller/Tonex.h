#pragma once
#include <Arduino.h>

class TonexController {
private:
  uint8_t currentPatch = 0;
  uint8_t previousPatch = 0;
  
  static constexpr uint8_t PATCH_MIN = 0;
  static constexpr uint8_t PATCH_MAX = 127;

public:
  uint8_t getPatch() const {
    return currentPatch;
  }

  bool patchHasChanged(){
    if(currentPatch != previousPatch){
      previousPatch = currentPatch;
      return true;
    }
    return false;
  }

  void nextPatch() {
    if (currentPatch < PATCH_MAX) {
      currentPatch++;
    }
  }

  void prevPatch() {
    if (currentPatch > PATCH_MIN) {
      currentPatch--;
    }
  }

  void sendPatch() {
    Serial.write(0xC0);              // Program Change, canal 1
    Serial.write(currentPatch);
  }

  // preenche o nome do patch no buffer fornecido
  void getPatchName(char* buffer) const {
    uint8_t bank = currentPatch / 3;
    uint8_t slot = (currentPatch % 3) + 1;

    char letter = 'A' + currentPatch;
    if (letter > 'Z') {
      letter = '?';
    }

    // gera string como "A1", "B2" etc.
    buffer[0] = letter;
    //buffer[1] = '0' + slot;
    buffer[1] = '\0'; // terminador nulo
  }
};