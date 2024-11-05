#include <Arduino.h>

// Define parameters
const int pwmChannel = 0;   // Channel number (0-15)
const int freq = 5000;      // Frequency in Hz
const int resolution = 8;   // Resolution (8 bits for 0-255 duty cycle range)
const int ledPin = 13;      // GPIO pin connected to the LED

void setup() {
  // Configure PWM functionalitiy for the channel
  ledcSetup(pwmChannel, freq, resolution);
  
  // Attach the PWM channel to the LED pin
  ledcAttachPin(ledPin, pwmChannel);
}

void loop() {
  // Increase brightness
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(pwmChannel, dutyCycle);
    delay(10);
  }

  // Decrease brightness
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(pwmChannel, dutyCycle);
    delay(10);
  }
}
