#include <LiquidCrystal_I2C.h>

// LCD I2C config
LiquidCrystal_I2C lcd(0x27, 16, 2);
int cursorLine = 0;

//---------------------MENU STRUCT--------------
typedef struct MenuNode {
    char name[15];
    MenuNode* submenu[4];
    int submenuCount;
    MenuNode* parent;
    bool selected;
} MenuNode;

MenuNode* mainMenu = (MenuNode*) malloc(sizeof(MenuNode));
MenuNode* cursor = NULL;
//--------LCD SCREEN-------
MenuNode* LCDline0 = NULL;
MenuNode* LCDline1 = NULL;
//-------------------------

bool nodeExists(MenuNode* node){//EXISTS, NOT NULL?
    if (node == NULL || node->parent == NULL) return false;
    return true;
}

int calculateNodePos(MenuNode* node){//Calcula posicao do nó usando o pai
  if(!nodeExists(node)) return -1; //EXISTS, NOT NULL?

  MenuNode* parent = node->parent; //PARENT EXISTS!

  for(int i=0;i< parent->submenuCount;i++){
    if(parent->submenu[i] == node)
      return i; //RETURN NODE POS
  }
  return -1;
}

bool validMove(MenuNode* node, int move, int nodepos){ //NEXT NODE EXISTS?
  if(!nodeExists(node)) return false;//já verifica nulidade de parent
  if(move == 1)
     return ((nodepos+1) < node->parent->submenuCount);
  else if (move == 0)
     return ((nodepos-1) >=0);
  else return false;
}
void navigate(MenuNode* node, int move) {
    /*
  *****MOVE****
    0 -> Previous
    1 -> Next
    2 -> Confirm
  *****************
  */
  int nodepos = calculateNodePos(node);
  if(nodepos == -1) return;//NODE DOES NOT EXISTS
  MenuNode* parent = node->parent;//PARENT EXISTS!

  if(move == 1 && validMove(node, move, nodepos)){//1 -> Next
      if(cursorLine == 0)
        cursorLine = 1;
      cursor = parent->submenu[nodepos+1];
    }
}

void lcdInit(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Patch# ");
  //lcd.clear();
}

void renderMenu(){
  lcd.setCursor(0, 0);
  lcd.print("hacker# ");
  lcd.setCursor(0, 1);
  lcd.print("Patch# ");
}



