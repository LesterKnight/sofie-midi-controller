#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

enum MenuActions {
  NEXT,
  PREVIOUS,
  ENTER,
  BACK
};

class Menu {
private:
  Menu* cursor = nullptr;
  int cursorLine = 0;
  static LiquidCrystal_I2C lcd;
  bool root = false;

public:
  static const int submenuLen = 7;
  char name[15];
  Menu* submenu[submenuLen];
  int submenuCount = 0;
  Menu* parent;
/*
  static void log(const char* message) { //   Menu::log("Iniciado!");
    Serial.println(message);
  }
*/
  Menu(const char* label = "MainMenu", Menu* parentNode = nullptr, bool isRoot = true) {
    
    strncpy(name, label, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    parent = parentNode;
    root = isRoot;
    for (int i = 0; i < submenuLen; i++) submenu[i] = nullptr;
  }

  int length() {
    int count = 0;
    for (int i = 0; i < submenuLen; i++) {
      if (submenu[i] != nullptr) count++;
      else break;
    }
    return count;
  }

  static bool nodeExists(Menu* node) {
    return node != nullptr && node->parent != nullptr;
  }

  static int calculateNodePos(Menu* node) {
    if (!nodeExists(node)) return -1;
    Menu* parent = node->parent;
    int count = parent->length();
    for (int i = 0; i < count; i++) {
      if (parent->submenu[i] == node) return i;
    }
    return -1;
  }

bool validMove(MenuActions move, int nodePos) {
  if (move == NEXT)
    return (nodePos + 1) < parent->length()-1;
  else if (move == PREVIOUS)
    return (nodePos - 1) >= 0;
  else
    return false;
}

void navigate(MenuActions move) {
  int nodePos = calculateNodePos(cursor);
  if (nodePos == -1) return;

  Menu* parentNode = cursor->parent;

  if (move == NEXT && validMove(move, nodePos)) {
    if (cursorLine == 0) cursorLine = 1;
    cursor = parentNode->submenu[nodePos + 1];
    renderMenu();
    Serial.println("move");
  } 
  else if (move == PREVIOUS && validMove(move, nodePos)) {
    Serial.println("move back start");
    if (cursorLine == 1) cursorLine = 0;
    cursor = parentNode->submenu[nodePos - 1];
    renderMenu();
    Serial.println("move back");
  }
}

  Menu* addChild(const char* label) {
    if (submenuCount >= submenuLen) return nullptr;
    Menu* child = new Menu( label, this, false);
    submenu[submenuCount++] = child;
    return child;
  }

  void init(){
    lcd.init();
    lcd.clear();
    lcd.backlight();
    cursor = addChild("Laranja");
    addChild("Abacaxi");
    addChild("Tamarindo");
    addChild("pEsSeGo");
    addChild("BANANA");
    addChild("VOLTAR");
    renderMenu();
  }

  void renderMenu() {
    lcd.clear();
    int pos = calculateNodePos(cursor);


    if (cursorLine == 0) {
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.print(cursor->name);
      if (pos + 1 < cursor->parent->length()) {
        lcd.setCursor(0, 1);
        lcd.print(cursor->parent->submenu[pos + 1]->name);
      }
    }
    else if (cursorLine == 1){
      lcd.setCursor(0, 0);
      lcd.print(cursor->parent->submenu[pos - 1]->name);
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.print(cursor->name);
    }
  }
  
};
LiquidCrystal_I2C Menu::lcd(0x27, 16, 2);
