#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> 

#define RETRY_TIME 500
#define POST_DATA_TRANSFER_TIMEOUT 100
#define BUFFER_LEN 30
#define BLE_SERVICE_UUID "2658a61b-6efd-4760-9f41-0e8489222429"
#define BLE_CHARACTERISTIC_UUID "ef9b96c4-c17f-45cb-bc57-0eefeaff93fd" 
#define BLE_NAME "LorenzzonGato"


void setup() {
  Serial.begin(9600);
  while (!Serial);

  // INICIALIZA O SENSOR

  if (!IMU.begin()) {
    Serial.println("Failed to Init IMU");
    while (1);
  }

  // INICIALIZA O BLE

  if (!BLE.begin()) {
    Serial.println("Failed to Init BLE");
    while (1);
  }
  
}

void loop() {
  // start scanning for peripherals
  BLE.scanForUuid(BLE_SERVICE_UUID);
  
  BLEDevice peripheral = BLE.available();
  
  //check if network was found 
  if (!peripheral) {
    Serial.print("No Network Found with Adress ");
    Serial.println(peripheral.address());
    Serial.println("Retrying in 0.5 second");
    delay(RETRY_TIME);
    return;
  }
  
  //check if network has the expected name
  if (peripheral.localName() != BLE_NAME) {
    Serial.println("Network Found But Incorrect name");
    Serial.println("Retrying in 0.5 second");
    delay(RETRY_TIME);
    return;
  }

  BLE.stopScan();

  controlLed(peripheral);

  // peripheral disconnected, start scanning again
  Serial.println("Network Disconnected");
  Serial.println("Retrying in 0.5 second");
  delay(RETRY_TIME);
}

float ax=0, ay=0, az=0;
float gx=0, gy=0, gz=0;
void tryReadSensor(){
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    float accelData[3] = {ax, ay, az};
  }  
    
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
    float gyroData[3] = {gx, gy, gz};
  }
}

void sendData(BLECharacteristic target){
      char buffer[BUFFER_LEN];
      memset(buffer,'\0',BUFFER_LEN);
      sprintf(buffer, "%.2f,%.2f,%.2f,%.2f", ax,ay,az, gz);

      target.writeValue(buffer);
}

void controlLed(BLEDevice peripheral) {

  if (!peripheral.connect()){
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (!peripheral.discoverAttributes()){
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic target = peripheral.characteristic(BLE_CHARACTERISTIC_UUID);
  if (!target) {
    Serial.println("Peripheral does not have characteristic!");
    peripheral.disconnect();
    return;
  } else if (!target.canWrite()) {
    Serial.println("Peripheral does not have a writable characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {
      tryReadSensor();    

      sendData(target);
      delay(POST_DATA_TRANSFER_TIMEOUT); 
  }
  return;
}

