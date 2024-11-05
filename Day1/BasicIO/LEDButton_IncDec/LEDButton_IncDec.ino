#define BUTTON_PIN 18          // Button to increment LEDs
#define REVERSE_BUTTON_PIN 5    // Button to decrement LEDs

const int ledPins[] = {13, 12, 14, 27, 26, 25, 33, 32};
int currentLed = 0;            // Tracks the current LED to turn on/off

int lastButtonState = HIGH;     // Stores the last state of BUTTON_PIN
int lastReverseButtonState = HIGH;  // Stores the last state of REVERSE_BUTTON_PIN

void setup() {
  Serial.begin(9600);

  // Set each LED pin as output and turn them off initially
  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Set the button pins as input with internal pull-up resistors
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(REVERSE_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Read button states
  int buttonState = digitalRead(BUTTON_PIN);
  int reverseButtonState = digitalRead(REVERSE_BUTTON_PIN);

  // Increment function: Turns on LEDs one by one
  if (lastButtonState == HIGH && buttonState == LOW) {
    if (currentLed < sizeof(ledPins) / sizeof(ledPins[0])) {
      digitalWrite(ledPins[currentLed], HIGH); // Turn on current LED
      currentLed++;  // Move to the next LED
    }
    delay(200); // Debounce delay
  }

  // Decrement function: Turns off LEDs one by one
  if (lastReverseButtonState == HIGH && reverseButtonState == LOW) {
    if (currentLed > 0) {
      currentLed--;  // Move to the previous LED
      digitalWrite(ledPins[currentLed], LOW); // Turn off current LED
    }
    delay(200); // Debounce delay
  }

  // Update the last button states
  lastButtonState = buttonState;
  lastReverseButtonState = reverseButtonState;
}
