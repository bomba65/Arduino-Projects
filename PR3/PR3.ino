int button1 = 2;
int button2 = 3;
#define pot A1
boolean b = false;
int leds [] = {5, 7, 9, 11, 4};
void setup() {
  Serial.begin (9600);
  Serial.setTimeout(10);
  for(int i = 0; i < 5; i++){
    pinMode(leds[i], OUTPUT);
  }
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(pot, INPUT);
}
int i = 0;
void loop() {
  int x;
  x = analogRead(pot)/4;
  Serial.println(x);
  if(x<50){
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], LOW);
    digitalWrite(leds[2], LOW);
    digitalWrite(leds[3], LOW);
    digitalWrite(leds[4], LOW);
    Serial.println("a");
  }
  else if(x<100){
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], HIGH);
    digitalWrite(leds[2], LOW);
    digitalWrite(leds[3], LOW);
    digitalWrite(leds[4], LOW);
    Serial.println("b");
  }
  else if(x<150){
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], HIGH);
    digitalWrite(leds[2], HIGH);
    digitalWrite(leds[3], LOW);
    digitalWrite(leds[4], LOW);
  }
 
  else if(x<200){
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], HIGH);
    digitalWrite(leds[2], HIGH);
    digitalWrite(leds[3], HIGH);
    digitalWrite(leds[4], LOW);
  }
  
  else if(x<255){
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], HIGH);
    digitalWrite(leds[2], HIGH);
    digitalWrite(leds[3], HIGH);
    digitalWrite(leds[4], HIGH);
  }

}
