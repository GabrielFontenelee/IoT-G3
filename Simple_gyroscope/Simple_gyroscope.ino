#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> 
#define BUFFER_LEN 30

BLEService imuService("2658a61b-6efd-4760-9f41-0e8489222429");

BLECharacteristic accelCharacteristic("ef9b96c4-c17f-45cb-bc57-0eefeaff93fd", BLERead | BLENotify, sizeof(float) * 3);
BLECharacteristic gyroCharacteristic("another-uuid-here", BLERead | BLENotify, sizeof(float) * 3);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // INICIALIZA O SENSOR

  if (!IMU.begin()) {
    Serial.println("Falha ao iniciar o IMU!");
    while (1);
  }

  // INICIALIZA O BLE

  if (!BLE.begin()) {
    Serial.println("Falha ao iniciar o BLE!");
    while (1);
  }

  // start scanning for peripherals
  BLE.scanForUuid("2658a61b-6efd-4760-9f41-0e8489222429");
  Serial.println("Fim do setup!");

}

void loop() {
 
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "LorenzzonGato") {
      return;
    }
    //Serial.println("Chegou no envio");
    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("2658a61b-6efd-4760-9f41-0e8489222429");
  }
}

float ax=0, ay=0, az=0;
float gx=0, gy=0, gz=0;


void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic teste = peripheral.characteristic("ef9b96c4-c17f-45cb-bc57-0eefeaff93fd");
  if (!teste) {
    Serial.println("Peripheral does not have LED characteristic!");
    peripheral.disconnect();
    return;
  } else if (!teste.canWrite()) {
    Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {

    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(ax, ay, az);
        float accelData[3] = {ax, ay, az};
      }
      
    
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gx, gy, gz);
        float gyroData[3] = {gx, gy, gz};
        gyroCharacteristic.writeValue((byte*)gyroData, sizeof(gyroData));
      }
      
      char buffer[BUFFER_LEN];
      memset(buffer,'\0',BUFFER_LEN);
      sprintf(buffer, "%.2f,%.2f,%.2f,%.2f", ax,ay,az, gz);

      teste.writeValue(buffer);

      delay(100); // Ajuste conforme necessário

  }

  Serial.println("Peripheral disconnected");
}

