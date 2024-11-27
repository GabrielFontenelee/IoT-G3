/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-mqtt
 */
/*
  Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
  Ported to Arduino ESP32 by Evandro Copercini
*/

#include <WiFi.h>
#include <MQTTClient.h>
#include <string.h>

#define CLIENT_ID "ESP32-001"  // CHANGE IT AS YOU DESIRE

const char WIFI_SSID[] = "Felipe2";              // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "natitqm7";           // CHANGE TO YOUR WIFI PASSWORD
const char MQTT_BROKER_ADRRESS[] = "192.168.17.29";  // CHANGE TO MQTT BROKER'S IP ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_USERNAME[] = "";  // CHANGE IT IF REQUIRED
const char MQTT_PASSWORD[] = "";  // CHANGE IT IF REQUIRED

// The MQTT topics that this device should publish/subscribe
#define PUBLISH_TOPIC "esp32-001/send"

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);
String message;

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("ESP32 - Connecting to Wi-Fi");
 
  rgbLedWrite(RGB_BUILTIN, 64, 64, 0);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  rgbLedWrite(RGB_BUILTIN, 0, 0, 64);
}

void setup() {
  // recieves from serial from another ESP32
  Serial1.begin(115200, SERIAL_8N1, 16, 17);
  // For debugging
  Serial.begin(115200);

  setupWifi();
 
  connectToMQTT();
  
  // setupBT();
  
}

void loop() {
  if (Serial1.available()) {
    message = Serial1.readStringUntil('\n');
    Serial.println("Recieved: " + message);
    sendToMQTT();
  }
}


void connectToMQTT() {
  // Connect to the MQTT broker
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);
  mqtt.setTimeout(30000);

  Serial.print("ESP32 - Connecting to MQTT broker");

  while (!mqtt.connect(CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }

  if (!mqtt.connected()) {
    Serial.println("ESP32 - MQTT broker Timeout!");
    rgbLedWrite(RGB_BUILTIN, 64, 0, 0);
    return;
  }
  Serial.println("\nConnected");
  rgbLedWrite(RGB_BUILTIN, 0, 64, 0);
}

void sendToMQTT() {
  if (!mqtt.connected()) {
    Serial.println("Not connected!");
    connectToMQTT();
    rgbLedWrite(RGB_BUILTIN, 0, 0, 64);
  }
  mqtt.publish(PUBLISH_TOPIC, message);
}
