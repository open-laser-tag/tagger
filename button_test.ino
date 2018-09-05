#include <SoftwareSerial.h>

int LED_PIN = 1;
int BUTTON_PIN = 0;

void setup() {
        pinMode(LED_PIN, OUTPUT); //LED on Model A   
        pinMode(BUTTON_PIN, INPUT);
}


void loop() {
  
  if (digitalRead(BUTTON_PIN) > 0) digitalWrite(LED_PIN, LOW);
  else  digitalWrite(LED_PIN, HIGH);
}

