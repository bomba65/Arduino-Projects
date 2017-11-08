#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int leftB = 6;
int rightB = 7;
int fireB = 8;
int startB = 9;

uint8_t aiImg[8] =
{
  B00000,
  B00000,
  B10100,
  B11110,
  B11110,
  B10100,
  B00000,
  B00000
};

uint8_t carImg[8] =
{
  B00000,
  B10100,
  B11110,
  B11111,
  B11111,
  B11110,
  B10100,
  B00000,
};

uint8_t wallImg[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
uint8_t bwallImg[8] =
{
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};

void setup(){
  Serial.begin (9600);

  randomSeed(analogRead(0));

  pinMode(leftB, INPUT);
  pinMode(rightB, INPUT);
  pinMode(fireB, INPUT);
  pinMode(startB, INPUT);
  
  lcd.begin(16, 2);
  lcd.createChar(0,carImg);
  lcd.createChar(1,aiImg);
  lcd.createChar(2,wallImg);
  lcd.createChar(3,bwallImg);

  lcd.setCursor(0,0);
  lcd.print("  Click Start");
  
}

int m[2][16];
int f[2][16];
int car = 1;
bool start = false;
bool fire = false;
bool fireCheck = true;

void loop(){
  if(digitalRead(startB) == HIGH) start = true;
  if(start){
    if(digitalRead(leftB) == HIGH) car = 1;
    if(digitalRead(rightB) == HIGH) car = 0;
    if(digitalRead(fireB) == HIGH) {
      if(fire == false && fireCheck) {
        f[car][0] = 1;
        fire = true;
        fireCheck = false;
      }
    } else fire = false;

    if(millis()%500<3){
      fillM();
      printM();
      check();  
    }
  }
}

void check(){
  if(m[car][0] != 0){
    lcd.clear();
    lcd.print("   GAME OVER!");
    for(int i = 0; i < 2; i++){
      for(int j = 0; j < 15; j++){
        m[i][j] = 0;
        f[i][j] = 0;
      }
    }
    car = 1;
    start = false;
  }
}

void fillM(){
  for(int j = 0; j < 15; j++){
    for(int i = 0; i < 2; i++){
      m[i][j] = m[i][j+1];
    }
  }
  
  int r = random(0,3);
  if(r == 0) {
    int q = random(0,2);
    m[q][15] = random(1,3);
    m[!q][15] = 0;
  }
  else{
    m[0][15] = 0;
    m[1][15] = 0;
  }

  for(int i = 0; i < 2; i++){
    for(int j = 15; j > 0; j--){
      if(f[i][15] == 1) fireCheck = true;
      f[i][j] = f[i][j-1];
      f[i][j-1] = 0;
      if(f[i][j] && m[i][j-1]){
        if(m[i][j-1] == 2) m[i][j-1] = 3;
        else m[i][j-1] = 0;
        f[i][j] = 0;
        fireCheck = true;
      }
      if(f[i][j] && m[i][j]){
        if(m[i][j] == 2) m[i][j] = 3;
        else m[i][j] = 0;
        f[i][j] = 0;
        fireCheck = true;
      }
    }
  }
}

void printM(){
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 16; j++){
      lcd.setCursor(j,i);
      if(f[i][j] == 1){
        lcd.print("-");
      }
      switch(m[i][j]){
        case 0:
          lcd.print(" ");
          break;
        case 1:
          lcd.print('\1');
          break;
        case 2:
          lcd.print('\2');
          break;
        case 3:
          lcd.print('\3');
          break;
        case 4:
          lcd.print('\0');
          break;
        case 5:
          lcd.print("-");
          break;
        default:
          lcd.print("?");
          break;
      }
    }
  }
  lcd.setCursor(0, car);
  lcd.print('\0');
}



// if(m[i][j+1] == 1) {
//        m[!i][j] = m[i][j+1];
//        i++;
//      }
//      else m[i][j] = m[i][j+1];
     

