#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "ZTE-AFL";
const char* password = "cicicuit?";
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Define LEDs using const int
const int led_1 = 13;
const int led_2 = 12;
const int led_3 = 14;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html { font-family: Arial; display: inline-block; text-align: center; }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    body { max-width: 600px; margin: 0px auto; padding-bottom: 25px; }
    .switch { position: relative; display: inline-block; width: 120px; height: 68px; }
    .switch input { display: none; }
    .slider { position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px; }
    .slider:before { position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px; }
    input:checked + .slider { background-color: #b30000; }
    input:checked + .slider:before { -webkit-transform: translateX(52px); transform: translateX(52px); }
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %BUTTONPLACEHOLDER%
  <script>
    function toggleCheckbox(element) {
      var xhr = new XMLHttpRequest();
      if(element.checked) {
        xhr.open("GET", "/update?output=" + element.id + "&state=1", true);
      } else {
        xhr.open("GET", "/update?output=" + element.id + "&state=0", true);
      }
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

// Function to replace placeholder with buttons for GPIO control
String processor(const String& var) {
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    buttons += "<h4>Output - GPIO 12</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"13\" " + outputState(led_1) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 14</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(led_2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"14\" " + outputState(led_3) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

// Function to return the current state of GPIO pin as "checked" or ""
String outputState(int output) {
  return digitalRead(output) ? "checked" : "";
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize GPIO pins
  pinMode(led_1, OUTPUT);
  digitalWrite(led_1, LOW);
  pinMode(led_2, OUTPUT);
  digitalWrite(led_2, LOW);
  pinMode(led_3, OUTPUT);
  digitalWrite(led_3, LOW);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Route to handle GPIO state update
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage1, inputMessage2;
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      int pin = inputMessage1.toInt();
      int state = inputMessage2.toInt();
      digitalWrite(pin, state);
    } else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop() {
}
