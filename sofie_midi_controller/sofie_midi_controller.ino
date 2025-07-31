#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>
// common I2C: 0x27 ou 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int BTN_THRESHOLD = 25;
const int BTN_1_PIN = 2;
int i = 0;
Bounce debouncer = Bounce();
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(BTN_1_PIN, INPUT_PULLUP);
  debouncer.attach(BTN_1_PIN); 
  debouncer.interval(BTN_THRESHOLD);
  Serial.begin(31250);
}
void loop() {
  debouncer.update();
  if (debouncer.fell()) {
    lcd.clear();
    i++;
    lcd.setCursor(0, 0);
    lcd.print("Patch# ");
    lcd.print(i);
    Serial.write(0xC0);// Program Change channel 1
    delay(50);
    Serial.write(i);
  }
}