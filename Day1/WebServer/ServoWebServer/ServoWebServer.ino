#include <WiFi.h>
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid = "ZTE-AFL";
const char* password = "cicicuit?";

// Create a Servo object
Servo myservo;

// Define the GPIO pin for the servo
const int servoPin = 13;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString = "90";  // Initial position value
int pos1 = 0;
int pos2 = 0;

// Timeout configuration
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Attach the servo on the servoPin
  myservo.attach(servoPin);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client) { // If a new client connects
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = ""; // Make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) { // Loop while the client's connected
      currentTime = millis();
      if (client.available()) { // If there's bytes to read from the client
        char c = client.read(); // Read a byte
        Serial.write(c); // Print it out in the serial monitor
        header += c;
        
        if (c == '\n') { // If the byte is a newline character
          if (currentLine.length() == 0) {
            // HTTP headers
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // HTML web page content
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>body { text-align: center; font-family: 'Trebuchet MS', Arial; margin-left:auto; margin-right:auto; }");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
            client.println("</head><body><h1>ESP32 with Servo</h1>");

            // Slider control for servo position
            client.println("<p>Position: <span id=\"servoPos\"></span></p>");
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\"" + valueString + "\"/>");

            // JavaScript for handling the slider
            client.println("<script>");
            client.println("var slider = document.getElementById('servoSlider');");
            client.println("var servoP = document.getElementById('servoPos'); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { servoP.innerHTML = this.value; };");
            client.println("$.ajaxSetup({timeout:1000});");
            client.println("function servo(pos) { $.get('/?value=' + pos + '&'); }</script>");

            client.println("</body></html>");

            // Check if the HTTP request contains position data
            if (header.indexOf("GET /?value=") >= 0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1 + 1, pos2);
              
              // Rotate the servo to the specified position
              myservo.write(valueString.toInt());
              Serial.println("Servo position: " + valueString);
            }

            // End HTTP response
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c; // Add it to the end of the currentLine
        }
      }
    }

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.\n");
  }
}