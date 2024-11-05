#include <esp32-hal-touch.h>
#include <Arduino.h>

#define LED_PIN 13
#define TOUCH_THRESHOLD 40 // Sensitivity threshold

RTC_DATA_ATTR int bootCount = 0; // Number of reboots
touch_pad_t touchPin;            // GPIO pin that triggered the wake-up

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);        // Start serial communication at 115200 baud rate
    delay(1000);                 // Delay to allow serial monitor to open
    
    ++bootCount;                 // Increment boot count on each reboot
    Serial.println("Boot number: " + String(bootCount));

    // Check wake-up reason
    touchPin = esp_sleep_get_touchpad_wakeup_status();

    if (touchPin == TOUCH_PAD_NUM5) {
        Serial.println("Touch detected on GPIO 12 (Touch Pad 5)");
    } else {
        Serial.println("Wakeup not by touchpad");
    }

    // Configure touch threshold and attach callback function to touch pads
    touchAttachInterrupt(T5, NULL, TOUCH_THRESHOLD);

    // Enable touch pad as a wake-up source
    esp_sleep_enable_touchpad_wakeup();

    Serial.println("Preparing to sleep now");
    Serial.flush();              // Ensure the serial buffer is empty before sleeping

    esp_deep_sleep_start();       // Enter deep sleep
}

void loop() {
    // This code will never be reached because ESP32 is in deep sleep
}
