#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x0C, 0xB8, 0x15, 0xF5, 0x6A, 0x8C};

// Define variables to store outgoing readings
const int numberOfSensors = 2;
int distance_cm[numberOfSensors];
int trigPin[numberOfSensors] = {2,4};
int echoPin[numberOfSensors] = {1,3};
long duration;
int lastData[numberOfSensors] = {0,0};
int currentData[numberOfSensors] = {0,0};
bool isChanged = true;

// Variable to store if sending data was successful
String success;

// Create a message to hold sensor readings
typedef struct struct_message
{
  int topdist;
  int bottomdist;
} struct_message;

struct_message distReadings;

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
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  for(int i = 0; i < numberOfSensors; i++)
  {
    pinMode(trigPin[i],OUTPUT);
    pinMode(echoPin[i],INPUT);
  }
  
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
}
 
void loop() {
  for(int currentSensor = 0; currentSensor<numberOfSensors; currentSensor++)
  {
    currentData[currentSensor] = getReadings(currentSensor);
    if(currentData[currentSensor] != lastData[currentSensor])
      isChanged = true;
    lastData[currentSensor] = currentData[currentSensor];
  }

  if(isChanged)
  {
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &distReadings, sizeof(distReadings));
     
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    isChanged = !isChanged;
  }
  delay(400);
}

int getReadings(int currentSensor){
  int retdist;
  digitalWrite(trigPin[currentSensor], LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin[currentSensor], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin[currentSensor], LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin[currentSensor], HIGH);
  // Calculating the distance
  switch(currentSensor)
  {
    case 0:
    if(duration * 0.034 / 2 <= 11)
      distReadings.topdist = 1;
    else
      distReadings.topdist = 0;
    retdist = distReadings.topdist;
    break;
  
    case 1:
    if(duration * 0.034 / 2 <= 11)
      distReadings.bottomdist = 1;
    else
      distReadings.bottomdist = 0;
    retdist = distReadings.bottomdist;
    break;
  }
   return retdist;
}
