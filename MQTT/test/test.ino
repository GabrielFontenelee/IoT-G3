#include <WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
// https://esp32io.com/tutorials/esp32-mqtt

const char WIFI_SSID[] = "Felipe2";
const char WIFI_PASSWORD[] = "natitqm7";

const char MQTT_BROKER_ADRRESS[] = "test.mosquitto.org";
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "Felipe-Loras-esp32-001";
const char MQTT_USERNAME[] = "";
const char MQTT_PASSWORD[] = "";

const char PUBLISH_TOPIC[] = "Felipe-Loras-esp32-001/loopback";
const char SUBSCRIBE_TOPIC[] = "Felipe-Loras-esp32-001/loopback";

const int PUBLISH_INTERVAL = 5000; // 5s

WiFiClient network;
MQTTClient mqtt = MQTTClient(256); 

unsigned long lastPublishTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  analogSetAttenuation(ADC_11db);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("ESP32 - Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  connectToMQTT();
}

void loop() {
  // put your main code here, to run repeatedly:
  mqtt.loop();

  if(millis() - lastPublishTime > PUBLISH_INTERVAL) {
    sendToMQTT();
    lastPublishTime = millis();
  }
}


void connectToMQTT() {
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);

  mqtt.onMessage(messageHandler);

  Serial.print("ESP32 - Connecting to MQTT broker");

  while(!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if(!mqtt.connected()) {
    Serial.println("ESP32 - MQTT broker timeout!");
    return;
  }

  if(mqtt.subscribe(SUBSCRIBE_TOPIC))
     Serial.print("ESP32 - Subscribed to the topic: ");
  else
    Serial.print("ESP32 - Failed to subscribe to the topic: ");

  Serial.println(SUBSCRIBE_TOPIC);
  Serial.println("ESP32 - MQTT broker Connected!");
} 
void sendToMQTT() {
  StaticJsonDocument<200> message;
  message["timestamp"] = millis();
  message["data"] = analogRead(0);  // Or you can read data from other sensors
  char messageBuffer[512];
  serializeJson(message, messageBuffer);

  mqtt.publish(PUBLISH_TOPIC, messageBuffer);

  Serial.println("ESP32 - sent to MQTT:");
  Serial.print("- topic: ");
  Serial.println(PUBLISH_TOPIC);
  Serial.print("- payload:");
  Serial.println(messageBuffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("ESP32 - received from MQTT:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);
}