#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height, in pixels
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // 0x3C is the I2C address for most OLED displays
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Loop forever if display initialization fails
  }

  // Clear the display buffer
  display.clearDisplay();

  // Display "Hello, World!" message
  display.setTextSize(2);             // Set text size
  display.setTextColor(SSD1306_WHITE); // Set text color
  display.setCursor(0, 10);            // Set cursor position
  display.println("Hello,,,,");
  display.display();                   // Show initial message on display
  delay(2000);                         // Wait for 2 seconds to show the message
}

void loop() {
  // Clear the display buffer
  display.clearDisplay();

  // Display "Hello,,,,,"
  display.setTextSize(2);               // Set text size
  display.setTextColor(SSD1306_WHITE);  // Set text color
  display.setCursor(0, 10);             // Set cursor position
  display.println("Hello,,,");

  // Generate and display a random number
  int randomNumber = random(0, 100);    // Generate random number between 0 and 99
  display.setTextSize(2);               // Set text size
  display.setCursor(0, 40);             // Set cursor position
  display.print("Number: ");
  display.print(randomNumber);

  // Show the updated content on display
  display.display();

  // Wait for 1 second before updating the number
  delay(1000);
}
