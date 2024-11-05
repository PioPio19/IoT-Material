#define BUTTON_PIN 18
#define LED_PIN 5

int led_state = LOW;
int button_state;
int last_button_state;

void setup() {
  Serial.begin(9600);

  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);

  // Set button pin as input with internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize button state
  button_state = digitalRead(BUTTON_PIN);
  last_button_state = button_state;
}

void loop() {
  // Save the previous button state
  last_button_state = button_state;

  // Read the current button state
  button_state = digitalRead(BUTTON_PIN);

  // Check if the button was just pressed (transition from HIGH to LOW)
  if (last_button_state == HIGH && button_state == LOW) {
    Serial.println("The button is pressed");

    // Toggle LED state
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
  }
}
