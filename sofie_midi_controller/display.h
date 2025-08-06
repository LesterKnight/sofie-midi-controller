#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

  enum MenuActions {
    NEXT,
    PREVIOUS,
    ENTER,
  };

  enum MenuType {
    ROOT,
    SUBMENU,
    FUNCTION,
    BACK
  };

class Menu {
private:
  Menu* cursor = nullptr;
  int cursorLine = 0;
  static LiquidCrystal_I2C lcd;
  MenuType type;

  int subMenulength() {
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
    int count = parent->subMenulength();
    for (int i = 0; i < count; i++) {
      if (parent->submenu[i] == node) return i;
    }
    return -1;
  }

  bool validMove(MenuActions move, int nodePos) {
    if (move == NEXT)
      return (nodePos + 1) < parent->subMenulength()-1;
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
      lcd.print(">");
      lcd.print(cursor->name);
      if (pos + 1 < cursor->parent->subMenulength()) {
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

public:
  static const int submenuLen = 7;
  char name[15];
  Menu* submenu[submenuLen];
  int submenuCount = 0;
  Menu* parent;

  Menu(const char* label = "MainMenu", Menu* parentNode = nullptr) {
    strncpy(name, label, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    parent = parentNode;
    for (int i = 0; i < submenuLen; i++) submenu[i] = nullptr;

    if(parentNode == nullptr)
      type = ROOT;
    else
      type = SUBMENU;
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
  void navigate(MenuActions move) {
    int nodePos = calculateNodePos(cursor);//remover validmove da loogica, rendermenu para o fim
    if (nodePos == -1 || !validMove(move, nodePos)) return;
    Menu* parentNode = cursor->parent;


switch (move) {
  case ENTER:
    // cursor->subMenulength()>0
    // cursor = cursor->parent;
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
    return false;
}
    renderMenu();
    delay(100);
  }

  Menu* addChild(const char* label) {
    if (submenuCount >= submenuLen) return nullptr;
    Menu* child = new Menu( label, this);
    submenu[submenuCount++] = child;
    return child;
  }
};
LiquidCrystal_I2C Menu::lcd(0x27, 16, 2);
