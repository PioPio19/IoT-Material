/*
  ESP-NOW Demo - Transmit
  esp-now-demo-xmit.ino
  Sends data to Responder
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/
 
// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
 
// MAC Address of responder - edit as required
// uint8_t broadcastAddress[] = {0xFC, 0xF5, 0xC4, 0x2B, 0x26, 0x04}; //beardboard
uint8_t broadcastAddress[] = {0x8C, 0xAA, 0xB5, 0x86, 0x23, 0x90}; //Development box
 
// Define a data structure
typedef struct struct_message {
  char a[40];
} struct_message;
 
// Create a structured object
struct_message myData;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
}
void setup() {
  
  // Set up Serial Monitor
  Serial.begin(115200);
 
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
 
  // Create test data
  
  // Format structured data
  strcpy(myData.a, "Welcome to the Workshop board A com8!");
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(2000);
}
