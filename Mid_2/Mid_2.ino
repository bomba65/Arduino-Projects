#include <LiquidCrystal.h>
#define pr(x) Serial.println(x)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
uint8_t eneImg[8] =
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
  B01110,
  B11111,
  B00000,
  B01010,
  B00100,
  B10001,
  B01110,
  B00000,
};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0,carImg);
  lcd.createChar(1,eneImg);
  
}



const int AAT = 10,OKT = 1;
int aat,okt;

void loop() {
  control();
  if(!aat--){
    action();
    aat = AAT;
  }
  check();
  if(!okt--){
    okact();
    okt = OKT;
  }
  check();
  print();
  delay(30);
}


//---------------------------------------

int m[2][17];
int car,bot,bott;
int ok[2];

void control(){
  car = analogRead(A1)>512;
  if(digitalRead(10) && !ok[0]){
    while(digitalRead(10));
    ok[0] = 1;
    ok[1] = car;
  }
}

void action(){
  for(int r = 0;r<2;r++){
    for(int i = 0;i<16;i++){
      m[r][i] = m[r][i+1];
    }
    m[r][16] = random(0,100)<17?2:0;
  }
  if(!bott){
    bot = random(0,2);
  }
  else bott--;
}

void okact(){
  if(ok[0]){
    ok[0]++;
    if(ok[0]==16)ok[0]=0;
  }
}

void check(){
  if(m[ok[1]][ok[0]]){
    m[ok[1]][ok[0]]--;
    ok[0] = 0;
  }
  if(m[car][0]){
    lcd.clear();
    lcd.print("   GAME OVER!");
    delay(1000);
  }
  if(!bott && ok[0]==15 && ok[1]==bot){
    bott = 5;
  }
}

void print(){
  for(int r = 0;r<2;r++){
    lcd.setCursor(0,r);
    for(int i = 0;i<17;i++){
      switch(m[r][i]){
        case 2:
          lcd.print("#");
          break;
        case 1:
          lcd.print("+");
          break;
        case 0:
          lcd.print(" ");
          break;
        default:
          lcd.print("?");
      }
    }
  }

  lcd.setCursor(0,car);
  lcd.print('\0');

  lcd.setCursor(15,bot);
  if(!bott)lcd.print('\1');

  lcd.setCursor(ok[0],ok[1]);
  if(ok[0]) lcd.print("-");
} 
