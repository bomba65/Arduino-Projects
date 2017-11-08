int led = 12;
void setup() {
  Serial.begin (9600);
  pinMode(led, OUTPUT);
}
int n;

void loop() {
  Serial.print("Enter int n: ");
  while(Serial.available()==0){ 
  }
  n = Serial.parseInt();
  Serial.println(n);

  for(int i = 0; i < n; i++){
    digitalWrite(led,HIGH);
    delay(250);
    digitalWrite(led,LOW);
    delay(250);
  }
}
