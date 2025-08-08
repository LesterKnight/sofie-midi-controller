#include <Wire.h>
#include <Bounce2.h>
#include "./Menu.h"
#include "./Tonex.h"

Bounce debouncer = Bounce();
Menu display;
const int BTN_THRESHOLD = 25;
const int pinA = 2;  // Pin A (CLK)
const int pinB = 3;  // Pin B (DT)
const int pinSW = 4;  // Pin SW
int lastCLK = HIGH;

TonexController tonex;
int i = 0;
void setup() {
  Serial.begin(31250);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  debouncer.attach(pinSW); 
  debouncer.interval(BTN_THRESHOLD);
  display.init();
  display.renderPatch(tonex);
}

void loop() {
  if(display.mode() == MENU){
    int currentCLK = digitalRead(pinA);
      if (currentCLK != lastCLK && currentCLK == LOW) {
        if (digitalRead(pinB) != currentCLK) {
          display.navigate(NEXT);
        } else {
          display.navigate(PREVIOUS);
        }
      }
    lastCLK = currentCLK;
    debouncer.update();
    if (debouncer.fell()) {
      display.navigate(ENTER);
    }
  }

  if(display.mode() == PRESET){
      int currentCLK = digitalRead(pinA);
      if (currentCLK != lastCLK && currentCLK == LOW) {
        if (digitalRead(pinB) != currentCLK) {
          tonex.nextPatch();
          delay(100);
          Serial.println(i++);
        } else {
          tonex.prevPatch();
          delay(100);
          Serial.println(i--);
        }
      }
    if(tonex.patchHasChanged())
        display.renderPatch(tonex);
  }

}