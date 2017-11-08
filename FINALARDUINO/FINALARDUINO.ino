#include <SoftwareSerial.h>
#include <Servo.h>


#define DEBUG true

int led = 13;
 
SoftwareSerial esp8266(8,9); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
const int temp = A0;
const int wsens = A1;
const int ldrPin = A2;
Servo myservo;
bool motor = false;

const int stepperPins[4] = {9, 10, 11, 12};
int delayTime = 5;


void setup()
{
  Serial.begin(115200);
  esp8266.begin(115200); // your esp's baud rate might be different
 
  
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80

  myservo.attach(8);
  myservo.write(20);

  for(int i=0; i<4; i++) pinMode(stepperPins[i], OUTPUT);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

}

void loop()
{ 

  int tValue = analogRead(temp);
  int wValue = analogRead(wsens);
  int lValue = analogRead(ldrPin);


  //if(wValue < 300) myservo.write(0);
  //else myservo.write(180);

  //if(tValue < 60) motor = true;
  //else motor = false;

  //if(lValue < 400) digitalWrite(led, HIGH);
  //else digitalWrite(led, LOW);

  if(motor){
    digitalWrite(stepperPins[0], LOW);
    digitalWrite(stepperPins[1], HIGH);
    digitalWrite(stepperPins[2], HIGH);
    digitalWrite(stepperPins[3], LOW);
    delay(delayTime);
    digitalWrite(stepperPins[0], LOW);
    digitalWrite(stepperPins[1], HIGH);
    digitalWrite(stepperPins[2], LOW);
    digitalWrite(stepperPins[3], HIGH);
    delay(delayTime);
    digitalWrite(stepperPins[0], HIGH);
    digitalWrite(stepperPins[1], LOW);
    digitalWrite(stepperPins[2], LOW);
    digitalWrite(stepperPins[3], HIGH);
    delay(delayTime);
    digitalWrite(stepperPins[0], HIGH);
    digitalWrite(stepperPins[1], LOW);
    digitalWrite(stepperPins[2], HIGH);
    digitalWrite(stepperPins[3], LOW);
    delay(delayTime);
  }
  
  if(esp8266.available()) // check if the esp is sending a message 
  {
    /*
    while(esp8266.available())
    {
      // The esp has data so display its output to the serial window 
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
    } */
    

     delay(1000);
 
     int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
     
     String cipSend = "AT+CIPSEND=";
     
     String webpage= " " + (String)tValue + " " + (String)wValue + " " + (String)lValue;
     Serial.println(webpage);
     
     
     cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
 
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,3000,DEBUG);

    
    
  }
}
  
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        if(c=='~') myservo.write(90);
        else if(c=='@') myservo.write(20);
        else if(c=='}') digitalWrite(led,HIGH);
        else if(c=='{') digitalWrite(led,LOW);
        else if(c=='[') {
          motor = true;
          Serial.println(motor);
          Serial.println("##############################");
        }
        else if(c==']') motor = false;
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
 
