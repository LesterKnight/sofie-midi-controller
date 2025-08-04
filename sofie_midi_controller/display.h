#include <LiquidCrystal_I2C.h>

// LCD I2C config
LiquidCrystal_I2C lcd(0x27, 16, 2);
int cursorLine = 0;

//---------------------MENU STRUCT--------------
typedef struct MenuNode {
    char name[15];
    MenuNode* submenu[4];
    int submenuCount = 0;
    MenuNode* parent;
} MenuNode;

MenuNode* mainMenu = (MenuNode*) malloc(sizeof(MenuNode));
MenuNode* cursor = NULL;

void createNode(char name[15], MenuNode* parent){
  MenuNode* item = (MenuNode*) malloc(sizeof(MenuNode));
  strcpy(item->name, name);
  item->parent = parent;
  parent->submenu[parent->submenuCount++] = item;
}

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
     return ((nodepos+1) <= node->parent->submenuCount);
  else if (move == 0)
     return ((nodepos-1) >=0);
  else return false;
}

void navigate(MenuNode* node, int move) {
  int nodepos = calculateNodePos(node);
  Serial.println("pos atual:");
  Serial.println(nodepos);

  if(nodepos == -1) return;//NODE DOES NOT EXISTS
  MenuNode* parent = node->parent;//PARENT EXISTS!



  if(move == 1 && validMove(node, move, nodepos+1)){//1 -> Next
      if(cursorLine == 0){
        cursorLine = 1;
      }
      cursor = parent->submenu[nodepos+1];
      
  }
}

void lcdInit(){
  lcd.init();
  lcd.backlight();
}

void renderMenu(){
  lcd.clear();
  if(cursorLine == 0){
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.print(cursor->name);
    int pos = calculateNodePos(cursor);
    lcd.setCursor(0, 1);
    lcd.print(cursor->parent->submenu[pos+1]->name);
  }
  else{
    lcd.setCursor(0, 1);
    lcd.print(">");
    lcd.print(cursor->name);
    int pos = calculateNodePos(cursor);
    lcd.setCursor(0, 0);
    lcd.print(cursor->parent->submenu[pos-1]->name);
  }
}

//-------------CREATE MENU
void menuInit(){
  createNode("Laranja", mainMenu);
  createNode("Abacaxi", mainMenu);
  createNode("Framboesa", mainMenu);
  createNode("VOLTAR", mainMenu);
  cursor = mainMenu->submenu[0];
}