#include <Wire.h>
#include <Bounce2.h>
#include "./display.h"


const int BTN_THRESHOLD = 25;
const int BTN_1_PIN = 2;
int i = 0;
int VOL = 0;
int BOOST = 0;

Bounce debouncer = Bounce();

void setup() {
  Serial.begin(31250);

  pinMode(BTN_1_PIN, INPUT_PULLUP);
  debouncer.attach(BTN_1_PIN); 
  debouncer.interval(BTN_THRESHOLD);

  lcdInit();
}
void loop() {
  debouncer.update();
  if (debouncer.fell()) {
    renderMenu();
    //i++;
    //Serial.write(0xC0);// Program Change channel 1
    //delay(50);
    //Serial.write(i);
  }
}