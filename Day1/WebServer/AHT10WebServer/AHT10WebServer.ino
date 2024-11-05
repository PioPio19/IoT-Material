/*********
  Manual Book Internet of Things, page 
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
float suhu, kelembaban;

const unsigned long eventInterval = 400;
unsigned long previousTime2 = 0;
unsigned long previousTime = 0;

const long timeoutTime = 2000;  // Timeout time in milliseconds (2000ms = 2s)

// Replace with your network credentials
const char* ssid = "ZTE-AFL";
const char* password = "cicicuit?";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  aht.begin();
  server.begin();
}

void loop() {
  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {  // If a new client connects
    Serial.println("New Client.");
    String currentLine = "";  // Make a String to hold incoming data from the client

    while (client.connected()) {  // Loop while the client's connected
      if (client.available()) {  // If there's bytes to read from the client
        char c = client.read();  // Read a byte
        header += c;
        
        if (c == '\n') {  // If the byte is a newline character
          // If the current line is blank, you got two newline characters in a row
          // That's the end of the client HTTP request, so send a response
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g., HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }");
            client.println(".button { background-color: #7D7A76; border: none; color: white; padding: 16px 40px; font-size: 30px; margin: 2px; cursor: pointer; }");
            client.println(".button2 {background-color: #195B6A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<br>");
            client.println("<h3>Data Sensor</h3>");
            client.println("<h3> Suhu: " + String(suhu) + " C </h3>");
            client.println("<h3> Kelembaban: " + String(kelembaban) + "% rH</h3>");
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            break;
          } else {
            currentLine = "";  // Clear currentLine if a newline is received
          }
        } else if (c != '\r') {
          currentLine += c;  // Add anything else but a carriage return to currentLine
        }
      }
    }

    // Clear the header variable and close the connection
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  // Sensor data update interval
  if (millis() - previousTime2 >= eventInterval) {
    previousTime2 = millis();
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);  // Populate temp and humidity objects with fresh data
    suhu = temp.temperature;
    kelembaban = humidity.relative_humidity;
  }
}
