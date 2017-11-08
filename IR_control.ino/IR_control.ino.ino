#include "IRremote.h"
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(9, 8);


const int trigPin = 16;
const int echoPin = 15;

int receiver = 14;
IRrecv irrecv(receiver);
decode_results results;


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String secretCode = "1111";
String prCode = "";
bool checkP = true;
bool checkA = false;
bool checkD = false;
bool checkC = false;


void getWifiResponse() {
  delay(100);
  String r = "";
  while (wifiSerial.available() > 0) {
    r += wifiSerial.readString();
  }
  Serial.print(r);
}

void setup() {
  Serial.begin(115200);
  wifiSerial.begin(115200);
  wifiSerial.println("AT");
  getWifiResponse();
  wifiSerial.println("AT+CIPMUX=0");
  getWifiResponse();
  wifiSerial.println("AT+CIPSTART=\"TCP\",\"192.168.43.18\",2222");
  getWifiResponse();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.begin(16, 2);

  irrecv.enableIRIn();
}

int duration;
int distanceCm;

void loop() {
  while (Serial.available() > 0) {
    wifiSerial.print((char)Serial.read());
  }
  while (wifiSerial.available() > 0) {
    Serial.print((char)wifiSerial.read());
  }

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  

  if (distanceCm < 20 && !checkP) {
    lcd.clear();
    lcd.print("Enter pass code: ");
    checkP = true;
  }
  if (distanceCm > 20 && checkP) {
    lcd.clear();
    lcd.print("Locked");
    checkP = false;
    checkA = false;
    checkC = false;
    prCode = "";
    wifiSerial.println("AT+CIPSEND=3");
    getWifiResponse();
    wifiSerial.println("1");
    getWifiResponse();
    //myservo.write(180);
  }

  

  if ( irrecv.decode( &results ) && checkP && !checkA) {
    if (results.value == 0xFFE01F) {
      //VOL-
      prCode = prCode.substring(0, prCode.length() - 1);
    }
    if (results.value == 0xFF906F) {
      //EQ
      prCode = "";
    }
    if (results.value == 0xFF6897) {
      prCode += 0;
    }
    if (results.value == 0xFF30CF) {
      prCode += 1;
    }
    if (results.value == 0xFF18E7) {
      prCode += 2;
    }
    if (results.value == 0xFF7A85) {
      prCode += 3;
    }
    if (results.value == 0xFF10EF) {
      prCode += 4;
    }
    if (results.value == 0xFF38C7) {
      prCode += 5;
    }
    if (results.value == 0xFF5AA5) {
      prCode += 6;
    }
    if (results.value == 0xFF42BD) {
      prCode += 7;
    }
    if (results.value == 0xFF4AB5) {
      prCode += 8;
    }
    if (results.value == 0xFF52AD) {
      prCode += 9;
    }

    if (!checkC) {
      lcd.clear();
      lcd.print("Enter pass code: ");
      lcd.setCursor(0, 1);
      lcd.print(prCode);
    }
    else {
      lcd.clear();
      lcd.print("Enter new pass:");
      lcd.setCursor(0, 1);
      lcd.print(prCode);
    }

    if (results.value == 0xFFC23D) {
      //>||
      if (!checkC) {
        checkA = true;
        if (prCode == secretCode) {
          lcd.clear();
          lcd.print("Access granted!");
          wifiSerial.println("AT+CIPSEND=3");
          getWifiResponse();
          wifiSerial.println("2");
          getWifiResponse();
          //myservo.write(90);
        }
        else {
          lcd.clear();
          lcd.print("Access denied!");
          checkD = true;
        }
      }
      else {
        checkA = true;
        if (prCode.length() > 7 && prCode.length() < 17) {
          lcd.clear();
          lcd.print("Your pass changed!");
          secretCode = prCode;
        }
        else {
          lcd.clear();
          lcd.print("min 8 and max 16");
          checkD = true;
        }
      }
    }
    irrecv.resume();
  }

  if ( irrecv.decode( &results ) && checkP) {
    if (results.value == 0xFF629D && checkA) {
      //CH
      checkC = true;
      checkA = false;

      prCode = "";
    }
    if (results.value == 0xFFA857 && checkD) {
      //VOL+
      prCode = "";
      checkA = false;
      checkD = false;
    }

    irrecv.resume();
  }
}
