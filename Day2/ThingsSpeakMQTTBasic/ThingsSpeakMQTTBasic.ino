char ssid[] = "YOUR_SSID"; //  Change this to your network SSID (name).
char pass[] = "YOUR_PASS";    // Change this your network password

//------ MQTT broker settings and topics
const char* mqtt_server = "mqtt3.thingspeak.com";


//-- published settings
const char* publishTopic ="channels/Channel_ID/publish";   //REPLACE THE Channel_ID WITH YOUR channel ID  

//-- subscribed settings Virtuino command 1   
const char* subscribeTopicFor_Command_1="channels/Channel_ID/subscribe/fields/field1";   //REPLACE THE Channel_ID WITH YOUR channel ID  

const unsigned long postingInterval = 20 * 1000; // Post data every 20 seconds.

//------------------------ Variables-----------------------------------
//-------------------------------------------------------------------------
#include <WiFi.h>  
#include <PubSubClient.h>
#include <WiFiClient.h>

WiFiClient espClient;
PubSubClient client(espClient);  // Download the library PubSubClient from the arduino library manager

unsigned long lastUploadedTime = 0;

//==========================================
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

//=====================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
      // client.connect("cliend ID", "username","password") Replace with your Thingspeak MQTT Device Credentials
    if (client.connect("cliend ID", "username","password")) {  
      Serial.println("connected");
      //client.subscribe(command1_topic);   
      client.subscribe(subscribeTopicFor_Command_1);   // subscribe the topics here
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//========================================= messageReceived
void messageReceived(char* topic, byte* payload, unsigned int length) {
  // Convert topic and payload to String
  String topicStr = String(topic);
  String payloadStr = "";

  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  Serial.println("Message arrived [" + topicStr + "] " + payloadStr);
}

//========================================= setup
//=========================================
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(1);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(messageReceived);
}


//========================================= loop
//=========================================
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
 
  if (millis() - lastUploadedTime > postingInterval) { // The uploading interval must be > 15 seconds 
    int sensorValue_1 = random(100); // replace with your sensor value
    
    String dataText = String("field1=" + String(sensorValue_1)+ "&status=MQTTPUBLISH");
    publishMessage(publishTopic,dataText,true);    
    lastUploadedTime = millis();
  }
  
 } 
 
//======================================= publising as string
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str()))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}
