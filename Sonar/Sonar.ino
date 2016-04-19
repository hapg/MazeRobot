#include "HC_SR04.h"

#define LED_PING_PIN 13

#define PING_PIN 7
#define ECHO_PIN 8

HC_SR04 sonar1 = HC_SR04(PING_PIN, ECHO_PIN);

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_PING_PIN, OUTPUT);
}

void loop() {  
  digitalWrite(LED_PING_PIN, HIGH);
  float distance = sonar1.PulseForInches();
  digitalWrite(LED_PING_PIN, LOW);
  
  Serial.println(distance);

  delay(100);
}

