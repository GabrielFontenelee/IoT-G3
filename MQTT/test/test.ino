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


#include <BLEDevice.h>
#include <BLEServer.h>
#include <WiFi.h>
#include <MQTTClient.h>

#define CLIENT_ID "ESP32-001"  // CHANGE IT AS YOU DESIRE

const char WIFI_SSID[] = "Felipe2";              // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "natitqm7";           // CHANGE TO YOUR WIFI PASSWORD
const char MQTT_BROKER_ADRRESS[] = "192.168.17.29";  // CHANGE TO MQTT BROKER'S IP ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_USERNAME[] = "";  // CHANGE IT IF REQUIRED
const char MQTT_PASSWORD[] = "";  // CHANGE IT IF REQUIRED

// The MQTT topics that this device should publish/subscribe
#define PUBLISH_TOPIC "esp32-001/send"
#define SUBSCRIBE_TOPIC "esp32-001/send"
#define PUBLISH_INTERVAL 5000  // 4 seconds

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);
unsigned long lastPublishTime = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "2658a61b-6efd-4760-9f41-0e8489222429"
#define CHARACTERISTIC_UUID "ef9b96c4-c17f-45cb-bc57-0eefeaff93fd"

BLECharacteristic *pCharacteristic;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
     
     
      String value = pCharacteristic->getValue();

      uint32_t int_val = (uint32_t) value.c_str();

      Serial.println("some value arrived");
      delay(100);
      Serial.println(value);
      delay(100);
      print_binary(int_val);
    }

    
    bool get_bit(uint32_t num, uint32_t position)
    {
      bool bit = num & (1 << position);
      return bit;
    }
    
    
    void print_binary(uint32_t num)
    {
      Serial.print("binary: ");
      for(int i = 31; i >= 0; i--)
      {
        Serial.print(get_bit(num, i));
      }
      Serial.println(" -- ");
    }
    
};

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("ESP32 - Connecting to Wi-Fi");
 
  rgbLedWrite(RGB_BUILTIN, 64, 0, 0);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  rgbLedWrite(RGB_BUILTIN, 0, 0, 64);
  connectToMQTT();

  // setup Bluetooth
  BLEDevice::init("LorenzzonGato");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks()); 
  pCharacteristic->setValue("hola");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  mqtt.loop();

  if (millis() - lastPublishTime > PUBLISH_INTERVAL) {
    sendToMQTT();
    lastPublishTime = millis();
  }
}


void connectToMQTT() {
  // Connect to the MQTT broker
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);

  // Create a handler for incoming messages
  mqtt.onMessage(messageHandler);

  Serial.print("ESP32 - Connecting to MQTT broker");

  while (!mqtt.connect(CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (!mqtt.connected()) {
    Serial.println("ESP32 - MQTT broker Timeout!");
    return;
  }

  // Subscribe to a topic, the incoming messages are processed by messageHandler() function
  if (mqtt.subscribe(SUBSCRIBE_TOPIC)) {
    Serial.print("ESP32 - Subscribed to the topic: ");
    rgbLedWrite(RGB_BUILTIN, 0, 64, 0);
  } else {
    Serial.print("ESP32 - Failed to subscribe to the topic: ");
    rgbLedWrite(RGB_BUILTIN, 64, 0, 0);
  }
  Serial.println(SUBSCRIBE_TOPIC);
  Serial.println("ESP32  - MQTT broker Connected!");
}


void sendToMQTT() {
  mqtt.publish(PUBLISH_TOPIC, "8,9,10,11");
}


void messageHandler(String &topic, String &payload) {
  Serial.println("ESP32 - received from MQTT:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);
}
