#include <Arduino.h>

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
}

void loop() {
  // Print a message to the serial monitor:
  Serial.println("Hello, World!");
  // Wait for a second:
  delay(1000);
}
