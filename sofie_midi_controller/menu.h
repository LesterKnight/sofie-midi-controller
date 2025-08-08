#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "./Tonex.h"

enum MenuActions {
  NEXT,
  PREVIOUS,
  ENTER
};

enum MenuType {
  ROOT,
  SUBMENU,
  FUNCTION,
  BACK
};

enum MenuMode {
  PRESET,
  MENU,
  PARAM,
};

class Menu {
private:
  static Menu* cursor;
  static MenuMode status;
  static MenuMode previousStatus;
  int cursorLine = 0;
  static LiquidCrystal_I2C lcd;
  MenuType type;

  void setCursorArrow() {
    byte arrow[8] = { B00000, B00100, B00110, B11111, B00110, B00100, B00000, B00000 };
    if (cursorLine < 2) {
      lcd.createChar(0, arrow);
      lcd.setCursor(0, cursorLine);
      lcd.write(byte(0));
    }
  }
  static bool nodeExists(Menu* node) {
    return node != nullptr && node->parent != nullptr;
  }
  static int calculateNodePos(Menu* node) {
    if (!nodeExists(node)) return -1;
    Menu* parent = node->parent;
    int count = parent->submenuCount;
    for (int i = 0; i < count; i++) {
      if (parent->submenu[i] == node) return i;
    }
    return -1;
  }
  bool validMove(MenuActions move, int nodePos) {
    if (move == NEXT)
      return (nodePos + 1) < cursor->parent->submenuCount;
    else if (move == PREVIOUS)
      return (nodePos - 1) >= 0;
    else if (move == ENTER && (cursor->type == SUBMENU || cursor->type == FUNCTION || cursor->type == BACK)) return true;
    else
      return false;
  }
  void setIntParam() {
    byte char0[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };  // empty
    byte char4[8] = { 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000 };  // 25%
    byte char5[8] = { 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000 };  // 50%
    byte char6[8] = { 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100 };  // 75%
    byte char7[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 };  // 100%
  }



public:
  static const int submenuLen = 10;
  char name[15];
  Menu* submenu[submenuLen];
  int submenuCount = 0;
  Menu* parent;

  Menu(const char* label = "MainMenu", Menu* parentNode = nullptr, MenuType menuType = SUBMENU) {
    strncpy(name, label, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    parent = parentNode;
    for (int i = 0; i < submenuLen; i++) submenu[i] = nullptr;
    type = (parentNode == nullptr) ? ROOT : menuType;
  }
  MenuMode mode() {
    return Menu::status;
  }
  bool modeHasChanged() {
    if (Menu::status != Menu::previousStatus) {
      Menu::previousStatus = Menu::status;
      return true;
    }
    return false;
  }
  void init() {
    lcd.init();
    lcd.clear();
    lcd.backlight();

    Menu* volume;
    volume = addSubmenu("Volume");
    volume->addSubmenu("Main");
    volume->addSubmenu("Boost");
    volume->addBackBtn();
    cursor = volume;
    addSubmenu("Delay");
    addSubmenu("Chrous");
    addSubmenu("Midi Channel");
    addSubmenu("Save");
    addBackBtn();
  }
  void renderMenu() {
    lcd.clear();
    int pos = calculateNodePos(cursor);
    if (cursorLine == 0) {
      setCursorArrow();
      lcd.print(cursor->name);
      if (pos + 1 < cursor->parent->submenuCount) {
        lcd.setCursor(0, 1);
        lcd.print(cursor->parent->submenu[pos + 1]->name);
      }
    } else if (cursorLine == 1) {
      lcd.setCursor(0, 0);
      lcd.print(cursor->parent->submenu[pos - 1]->name);
      setCursorArrow();
      lcd.print(cursor->name);
    }
  }
  Menu* addSubmenu(const char* label) {
    if (submenuCount >= submenuLen) return nullptr;
    Menu* child = new Menu(label, this);
    submenu[submenuCount++] = child;
    return child;
  }
  void addBackBtn() {
    if (submenuCount >= submenuLen && type != SUBMENU) return;
    Menu* backBtn = new Menu("_BACK", this, BACK);
    submenu[submenuCount++] = backBtn;
  }
  void openMenu() {
    cursorLine = 0;
    cursor = submenu[0];
    status = MENU;
    renderMenu();
  }
  void renderPresetScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    char patchName[4];
    TonexController::getPatchName(patchName);
    cursorLine = 0;
    lcd.print(patchName);
    status = PRESET;
  }
  void navigate(MenuActions move) {
    if (mode() == MENU) {
      int nodePos = calculateNodePos(cursor);
      if (nodePos == -1 || !validMove(move, nodePos)) return;
      Menu* parentNode = cursor->parent;
      switch (move) {
        case ENTER:
          {
            if (cursor->type == SUBMENU && cursor->submenuCount > 0) {
              cursor = cursor->submenu[0];
              cursorLine = 0;
              renderMenu();
            } else if (cursor->type == BACK) {
              if (parentNode->type == ROOT) {
                cursor = submenu[0];
                renderPresetScreen();
              } else {
                cursor = parentNode->parent->submenu[0];
                cursorLine = 0;
                renderMenu();
              }
            }
            delay(300);
          }
          break;
        case NEXT:
          {
            if (cursorLine == 0) cursorLine = 1;
            cursor = parentNode->submenu[nodePos + 1];
            renderMenu();
          }
          break;

        case PREVIOUS:
          if (cursorLine == 1) cursorLine = 0;
          cursor = parentNode->submenu[nodePos - 1];
          renderMenu();
          break;
      }
    }
  }
};

LiquidCrystal_I2C Menu::lcd(0x27, 16, 2);
Menu* Menu::cursor = nullptr;
MenuMode Menu::status = PRESET;
MenuMode Menu::previousStatus = MENU;