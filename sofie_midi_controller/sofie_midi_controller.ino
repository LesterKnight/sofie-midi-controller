#include <Wire.h>
#include <Bounce2.h>
#include "./display.h"

const int BTN_THRESHOLD = 25;
const int BTN_1_PIN = 2;
Bounce debouncer = Bounce();
int time = 2500;
Menu menu;
void setup() {
  Serial.begin(31250);
  pinMode(BTN_1_PIN, INPUT_PULLUP);
  debouncer.attach(BTN_1_PIN); 
  debouncer.interval(BTN_THRESHOLD);
  menu.init();


    menu.navigate(1);
    delay(time);
    menu.navigate(1);
    delay(time);
    menu.navigate(1);
    delay(time);
    menu.navigate(1);
    delay(time);
    menu.navigate(1);
    delay(time);

    
    menu.navigate(0);
    delay(time);
    menu.navigate(0);
    delay(time);
    menu.navigate(0);
    delay(time);
    menu.navigate(0);
    delay(time);
    menu.navigate(0);
    delay(time);


}

void loop() {
  debouncer.update();
  if (debouncer.fell()) {  
  }
}