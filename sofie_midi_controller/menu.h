#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

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

class Menu {
private:
  static Menu* cursor;
  int cursorLine = 0;
  static LiquidCrystal_I2C lcd;
  MenuType type;

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
    else if (move == ENTER &&
    ( 
      cursor->type == SUBMENU||
      cursor->type == FUNCTION||
      cursor->type == BACK
    )) return true;
    else
      return false;
  }

  void renderMenu() {
    lcd.clear();
    int pos = calculateNodePos(cursor);
    if (cursorLine == 0) {
      lcd.setCursor(0, 0);
      lcd.print(".");
      lcd.print(cursor->name);
      if (pos + 1 < cursor->parent->submenuCount) {
        lcd.setCursor(0, 1);
        lcd.print(cursor->parent->submenu[pos + 1]->name);
      }
    }
    else if (cursorLine == 1){
      lcd.setCursor(0, 0);
      lcd.print(cursor->parent->submenu[pos - 1]->name);
      lcd.setCursor(0, 1);
      lcd.print(".");
      lcd.print(cursor->name);
    }
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

  void init(){
    lcd.init();
    lcd.clear();
    lcd.backlight();

    cursor = addSubmenu("VEGETABLES");
    addSubmenu("NACHO");
    addSubmenu("TACO");
    Menu* frutas = addSubmenu("FRUIT");
    frutas->addSubmenu("PINEAPLE");
    frutas->addSubmenu("PEACH");
    frutas->addSubmenu("GRAPE");
    frutas->addBackBtn();

    renderMenu();
  }
  void navigate(MenuActions move) {
    int nodePos = calculateNodePos(cursor);//remover validmove da loogica, rendermenu para o fim
    if (nodePos == -1 || !validMove(move, nodePos)) return;
    Menu* parentNode = cursor->parent;

    switch (move) {
      case ENTER:
        if(cursor->type == SUBMENU && cursor->submenuCount >0){
          cursor = cursor->submenu[0];
          cursorLine = 0;
        }
        else if (cursor->type == BACK){
          cursor = parentNode->parent->submenu[0];
          cursorLine = 0;
        }
        break;

      case NEXT:
        if (cursorLine == 0) cursorLine = 1;
        cursor = parentNode->submenu[nodePos + 1];
        break;

      case PREVIOUS:
        if (cursorLine == 1) cursorLine = 0;
        cursor = parentNode->submenu[nodePos - 1];
        break;

      default:
        return;
    }
        renderMenu();
        delay(300);
  }

  Menu* addSubmenu(const char* label) {
    if (submenuCount >= submenuLen) return nullptr;
    Menu* child = new Menu( label, this);
    submenu[submenuCount++] = child;
    return child;
  }

  void addBackBtn() {
    if (submenuCount >= submenuLen && type != SUBMENU) return;
    Menu* backBtn = new Menu( "_BACK", this, BACK);
    submenu[submenuCount++] = backBtn;
  }

};
LiquidCrystal_I2C Menu::lcd(0x27, 16, 2);
Menu* Menu::cursor = nullptr;
