#include <WiFi.h>
#include "config.h"

const char* ssid = "";
const char* password = "";

AdafruitIO_Feed *floatChannel = io.feed("Float");

#define FLOAT1_PIN 16
#define FLOAT2_PIN 15
#define FLOAT3_PIN 14

bool isOverflow = false;

void WifiSetup() {
  Serial.println();
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
// Auto reconnect is set true as default
// To set auto connect off, use the following function
//    WiFi.setAutoReconnect(false);

  // Will try for about 10 seconds (20x 500ms)
  int tryDelay = 500;
  int numberOfTries = 20;

  // Wait for the WiFi event
  while (true) {
      
      switch(WiFi.status()) {
        case WL_NO_SSID_AVAIL:
          Serial.println("[WiFi] SSID not found");
          break;
        case WL_CONNECT_FAILED:
          Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
          return;
          break;
        case WL_CONNECTION_LOST:
          Serial.println("[WiFi] Connection was lost");
          break;
        case WL_SCAN_COMPLETED:
          Serial.println("[WiFi] Scan is completed");
          break;
        case WL_DISCONNECTED:
          Serial.println("[WiFi] WiFi is disconnected");
          break;
        case WL_CONNECTED:
          Serial.println("[WiFi] WiFi is connected!");
          Serial.print("[WiFi] IP address: ");
          Serial.println(WiFi.localIP());
          return;
          break;
        default:
          Serial.print("[WiFi] WiFi Status: ");
          Serial.println(WiFi.status());
          break;
      }
      delay(tryDelay);
      
      if(numberOfTries <= 0){
        Serial.print("[WiFi] Failed to connect to WiFi!");
        // Use disconnect function to force stop trying to connect
        WiFi.disconnect();
        return;
      } else {
        numberOfTries--;
      }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(FLOAT1_PIN, INPUT_PULLUP);
  pinMode(FLOAT2_PIN, INPUT_PULLUP);
  pinMode(FLOAT3_PIN, INPUT_PULLUP);
  
  WifiSetup();
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  // WifiSetup();
  // put your setup code here, to run once:

}

void loop() {
  io.run();

  int float1data = digitalRead(FLOAT1_PIN);
  int float2data = digitalRead(FLOAT2_PIN);
  int float3data = digitalRead(FLOAT3_PIN);

  if ( float1data == HIGH || float2data == HIGH || float3data == HIGH ) {
    isOverflow = true;
  }
  else {
    isOverflow = false;
  }

  if ( isOverflow ) {
    Serial.println("Overflowing! Fix Immediately or face destruction");
  }

  floatChannel->save( isOverflow ? 1 : 0 );

  delay(10000);

  // put your main code here, to run repeatedly:

}
