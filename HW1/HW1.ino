int leds [] = {12,10, 8, 6};
void setup() {
  Serial.begin (9600);
  for(int i = 0; i < 4; i++){
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  for(int i = 0; i < 4; i++){
    if(i==0){
      digitalWrite(leds[0],HIGH);
      delay(5000);
      digitalWrite(leds[0],LOW);
    }
    else if(i==2){
      digitalWrite(leds[2],HIGH);
      delay(3000);
      digitalWrite(leds[2],LOW);
      delay(250);
      for(int j = 0; j < 4; j++){
        digitalWrite(leds[2],HIGH);
        delay(250);
        digitalWrite(leds[2],LOW);
        delay(250);
      }
    }
    else{
      digitalWrite(leds[1],HIGH);
      delay(1000);
      digitalWrite(leds[1],LOW);
    }
  }  
}
