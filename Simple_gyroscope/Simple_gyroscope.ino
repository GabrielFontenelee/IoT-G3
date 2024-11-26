#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> 

BLEService imuService("2658a61b-6efd-4760-9f41-0e8489222429");

BLECharacteristic accelCharacteristic("ef9b96c4-c17f-45cb-bc57-0eefeaff93fd", BLERead | BLENotify, sizeof(float) * 3);
BLECharacteristic gyroCharacteristic("another-uuid-here", BLERead | BLENotify, sizeof(float) * 3);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Falha ao iniciar o BLE!");
    while (1);
  }

  if (!IMU.begin()) {
    Serial.println("Falha ao iniciar o IMU!");
    while (1);
  }

  imuService.addCharacteristic(accelCharacteristic);
  imuService.addCharacteristic(gyroCharacteristic);
  BLE.addService(imuService);

  BLE.setLocalName("Arduino IMU");
  BLE.advertise();
  Serial.println("Anunciando...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Conectado a: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Ler os dados dos sensores
      float ax, ay, az;
      float gx, gy, gz;

      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(ax, ay, az);
        float accelData[3] = {ax, ay, az};
        accelCharacteristic.writeValue((byte*)accelData, sizeof(accelData));
      }

      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gx, gy, gz);
        float gyroData[3] = {gx, gy, gz};
        gyroCharacteristic.writeValue((byte*)gyroData, sizeof(gyroData));
      }

      delay(100); // Ajuste conforme necessário
    }

    Serial.println("Desconectado");
  }

  BLE.poll();
}
