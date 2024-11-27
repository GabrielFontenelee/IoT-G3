/*
  Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
  Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "2658a61b-6efd-4760-9f41-0e8489222429"
#define CHARACTERISTIC_UUID "ef9b96c4-c17f-45cb-bc57-0eefeaff93fd"

BLECharacteristic *pCharacteristic;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
     
     
      String value = pCharacteristic->getValue();

      uint32_t int_val = (uint32_t) value.c_str();

      //Serial.println("some value arrived");
      Serial1.println(value);
      //Serial1.println('\0');
      //Serial.println(value);
      //print_binary(int_val);
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
  //Serial.begin(115200, 1, 2, 3);
  Serial1.begin(115200, SERIAL_8N1, 16, 17); 
  Serial.println("Starting BLE work!");

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

int count = 0;
void loop() {
  // put your main code here, to run repeatedly:
  //delay(2000);
  while(1) {}
  //pCharacteristic->setValue("chao");
  //count++;
}