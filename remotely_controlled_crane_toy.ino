#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

//char auth[] = "xxx";
//char ssid[] = "xxx";
//char pass[] = "xxx";

#include "config.h"

BlynkTimer timer;
Servo strela, block, zahvat;
int strelaServoPin = D6;
int blockServoPin = D7;
int zahvatServoPin = D8;

// крайние положения сервы захвата
int zahvatLimitMin = 33;
int zahvatLimitMax = 180;

// Подключения мотора башни
int enA = D4;
int in1 = D1;
int in2 = D2;

// управление сервой стрелы
BLYNK_WRITE(V3) {
int pinValue = param.asInt();
  if(pinValue >= -150 && pinValue <= 150) {
    strela.detach();
  }
  else if(pinValue < -150 || pinValue > 150) {
    strela.attach(strelaServoPin);
    strela.write(pinValue);
  }
}

// управление сервой подьема
BLYNK_WRITE(V2) {
int pinValue = param.asInt();
  if(pinValue >= -150 && pinValue <= 150) {
    block.detach();
  }
  else if(pinValue < -150 || pinValue > 150) {
    block.attach(blockServoPin);
    block.write(pinValue);
  }
}

// управление сервой захвата
BLYNK_WRITE(V4) {
  int pinValue = param.asInt();
  if(pinValue == 0) {
    zahvat.detach();
  }
  else {
    zahvat.attach(zahvatServoPin);
    zahvat.write(pinValue);
  }
}

// управление мотором поворота башни
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  if(pinValue > -2 && pinValue < 2) {
    turnOffMotor();
  }
  else if(pinValue == -2) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if(pinValue == 2) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
}

void turnOffMotor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void setup() {
  // Set all the motor control pins to outputs
  //pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  analogWrite(enA, 255);
  turnOffMotor();
  
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

}

void loop() {
  Blynk.run(); // You can inject your own code or combine it with other sketches.
  timer.run();
}
