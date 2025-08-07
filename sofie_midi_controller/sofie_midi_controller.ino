#include <Wire.h>
#include <Bounce2.h>
#include "./menu.h"

Bounce debouncer = Bounce();
Menu menu;
const int BTN_THRESHOLD = 25;
const int pinA = 2;  // Pino A (CLK)
const int pinB = 3;  // Pino B (DT)
const int pinSW = 4;  // Pino SW
int lastCLK = HIGH;

void setup() {
  Serial.begin(31250);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  debouncer.attach(pinSW); 
  debouncer.interval(BTN_THRESHOLD);
  menu.init();
}

void loop() {
  int currentCLK = digitalRead(pinA);
    if (currentCLK != lastCLK && currentCLK == LOW) {
      if (digitalRead(pinB) != currentCLK) {
        menu.navigate(NEXT);
      } else {
        menu.navigate(PREVIOUS);
      }
    }
  lastCLK = currentCLK;

  debouncer.update();
  if (debouncer.fell()) {
    menu.navigate(ENTER);
  }
}