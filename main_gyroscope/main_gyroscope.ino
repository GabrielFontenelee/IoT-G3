#include <Arduino_LSM9DS1.h>

float x_a, y_a, z_a;
float x_g, y_g, z_g;
uint8_t movement = 0;
const float threshold = 1.0; // Ajuste conforme necessário

void processMovement(uint8_t movement);

void setup() {
  // put your setup code here, to run once:

  //Serial begin
  Serial.begin(9600);

  Serial.println("Serial initialized IMU ! \n");
  //IMU.begin();
  if(!IMU.begin()){
    Serial.println("Failed to initialize IMU ! \n");
    while(1);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

//Serial.println("Hello IMU ! \n");

  // Take measures
if (IMU.accelerationAvailable()){
    IMU.readAcceleration(x_a, y_a, z_a);

    movement = 0;

    if (x_a > threshold) {
      movement |= 0x01; // Direita
    } else if (x_a < -threshold) {
      movement |= 0x02; // Esquerda
    }

    if (z_a > threshold) {
      movement |= 0x04; // Ascendente
    } else if (z_a < -threshold) {
      movement |= 0x08; // Descendente
    }
  }

  if(IMU.gyroscopeAvailable()){
    IMU.readGyroscope(x_g, y_g, z_g);

    if (z_g > threshold) {
      movement |= 0x10; // Circular horário
    } else if (z_g < -threshold) {
      movement |= 0x20; // Circular anti-horário
    }
  }
  
  processMovement(movement);
}

void processMovement(uint8_t movement){

  if(movement == 0x00){
    printf("No movement detected !\n");
    return;
  }

  uint8_t red = 0, green = 0, blue = 0;

  switch (movement){

    case 0x04:
      Serial.println("Ascending movement\n");
      //rgbLedWrite(RGB_BUILTIN, 255, 0, 0);  // Red
      break;

    case 0x02:
      Serial.println("Left movement \n");
      //rgbLedWrite(RGB_BUILTIN, 0, 255, 0);  // Green
    break;

    case 0x01:
      Serial.println("Right movement \n");
      //rgbLedWrite(RGB_BUILTIN, 0, 0, 255);  // Blue
    break;

    case 0x08:
      Serial.println("Descending movement \n");
      //rgbLedWrite(RGB_BUILTIN, 255, 255, 0);  // Yellow
    break;

    case 0x20:
      Serial.println("Circle anti clock wise \n");
      //rgbLedWrite(RGB_BUILTIN, 255, 255, 255);  // White
      delay(100);
      //rgbLedWrite(RGB_BUILTIN, 0, 0, 0);  // Off
      delay(100);
      //rgbLedWrite(RGB_BUILTIN, 255, 255, 255);  // White
    break;

    case 0x10:
      Serial.println("Circle clock wise : Fix white \n");
      //rgbLedWrite(RGB_BUILTIN, 255, 255, 255);  // White

    break;

    default:
      Serial.println("Case not related \n");
      break;

    }
}

