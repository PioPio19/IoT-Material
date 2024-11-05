const int wakeupPin = 33;  // GPIO 4 for external wake-up
RTC_DATA_ATTR int bootCount = 0;  // Number of reboots

void setup()
{
  Serial.begin(115200);  // Start serial port at 115200 baud rate
  pinMode(wakeupPin, INPUT_PULLUP);  // Declaring the pin with the push button as INPUT_PULLUP
  bootCount++;  // Increment the number of boots by 1
  Serial.println("Boot number: " + String(bootCount));  // Print the boot number

  esp_sleep_enable_ext0_wakeup((gpio_num_t)wakeupPin, LOW);  // Configure external wake-up
  delay(1000);  // Adding a 1 second delay to avoid multiple presses

  Serial.println("ESP32 going to sleep now.");  // Print a statement before entering deep sleep
  esp_deep_sleep_start();  // Enter deep sleep mode
}

void loop()
{

}