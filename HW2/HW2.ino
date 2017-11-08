void setup() {
  Serial.begin (9600);
  Serial.setTimeout(10);
}
void loop() {
  Serial.print("Enter something: ");
  while(Serial.available()==0){ 
  }
  String n = Serial.readString();
  n += "-OK";
  Serial.println(n);
}

