#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

const int numberOfSensors = 2;
// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x0C, 0xB8, 0x15, 0xF2, 0xF1, 0x0C};

// Define variables to store incoming readings
int distance_cm[numberOfSensors];
const int top = 1;
const int bottom = 0;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message
{
  int topdist;
  int bottomdist;
} struct_message;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
//  Serial.print("Bytes received: ");
//  Serial.println(len);
  distance_cm[top] = incomingReadings.topdist;
  distance_cm[bottom] = incomingReadings.bottomdist;
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {  
  updateDisplay();
  delay(2000);
}

void updateDisplay(){
  // Display Readings in Serial Monitor
//  Serial.println("INCOMING READINGS");
  Serial.print("top");
  Serial.print(distance_cm[top]);
//  Serial.println(" cm");
  Serial.print("bottom");
  Serial.print(distance_cm[bottom]);
//  Serial.println(" cm");
  Serial.println();
}
