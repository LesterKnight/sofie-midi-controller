#include <Wire.h>
#include <Bounce2.h>
#include "./menu.h"

const int BTN_THRESHOLD = 25;
const int BTN_1_PIN = 2;

Bounce debouncer = Bounce();
int time = 200;
Menu menu;
void setup() {
  Serial.begin(31250);
  pinMode(BTN_1_PIN, INPUT_PULLUP);
  debouncer.attach(BTN_1_PIN); 
  debouncer.interval(BTN_THRESHOLD);
  menu.init();

  for (int i = 0; i < 3; i++) {
    menu.navigate(NEXT);
    delay(time);
  }
  menu.navigate(ENTER);
  for (int i = 0; i < 4; i++) {
    menu.navigate(NEXT);
    delay(time);
  }
  delay(5000);
  menu.navigate(ENTER);
  
}

void loop() {
  debouncer.update();
  if (debouncer.fell()) {  
  }
}