#include <Wire.h>
#include <Bounce2.h>
#include "./Menu.h"
#include "./Tonex.h"

Bounce debouncer = Bounce();
Menu display;
const int BTN_THRESHOLD = 25;
const int pinA = 2;   // Pin A (CLK)
const int pinB = 3;   // Pin B (DT)
const int pinSW = 4;  // Pin SW
int lastCLK = HIGH;

int i = 0;
void setup() {
  Serial.begin(31250);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  debouncer.attach(pinSW);
  debouncer.interval(BTN_THRESHOLD);
  display.init();
}
void loop() {
  int currentCLK = digitalRead(pinA);

  //BOTAO DEVE IR PARA A CLASSE MENU TAMBEM
  debouncer.update();
  if (debouncer.fell()) {  //BOTAO
    switch (display.mode()) {
      case PRESET:
        display.openMenu();
        break;
      case MENU:
        display.navigate(ENTER);
        break;
      case PARAM:
        //display.navigate(ENTER);
        break;
    }
  }
  //-----------------MENU SCREEN----------------------
  if (display.mode() == MENU) {
    if (currentCLK != lastCLK && currentCLK == LOW) {
      if (digitalRead(pinB) != currentCLK)
        display.navigate(NEXT);
      else
        display.navigate(PREVIOUS);
    }
  }
  //-----------------PRESET SCREEN----------------------
  if (display.mode() == PRESET) {
    if (currentCLK != lastCLK && currentCLK == LOW) {
      if (digitalRead(pinB) != currentCLK) {
        TonexController::nextPatch();
      } else {
        TonexController::prevPatch();
      }
    }
    if (TonexController::patchHasChanged() || display.modeHasChanged())
      display.renderPresetScreen();
  }
  //-----------------PARAM SCREEN----------------------
  if (display.mode() == PARAM) {
    if (currentCLK != lastCLK && currentCLK == LOW) {
      if (digitalRead(pinB) != currentCLK) {
        display.setIntParam(1);
      } else {
        display.setIntParam(-1);
      }
    }
  }




  lastCLK = currentCLK;
}
