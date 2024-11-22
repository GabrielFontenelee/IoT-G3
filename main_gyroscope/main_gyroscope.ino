#include <Arduino_LSM6DS3.h>

float x, y, z;
float x_g, y_g, z_g;

void setup() {
  // put your setup code here, to run once:

  //Serial begin
  Serial.begin(115200);

  //IMU.begin();
  if(!IMU.begin()){
    Serial.println("Failed to initialize IMU ! \n");
    while(1);
  }

}

void loop() {
  // put your main code here, to run repeatedly:


  // Take measures

  if (IMU.accelerationAvailable()){
    IMU.readAcceleration(x, y, z);
  }

  if(IMU.gyroscopeAvailable()){
    IMU.readGyroscope(x_g,y_g,z_g);
  }

  switch data{

    case 1:
      Serial.println("Ascending movement\n");
      rgbLedWrite(RGB_BUILTIN, 255, 0, 0);  // Red
      break;

    case 2:
      Serial.println("Left movement \n");
      rgbLedWrite(RGB_BUILTIN, 0, 255, 0);  // Green
    break;

    case 3:
      Serial.println("Right movement \n");
      rgbLedWrite(RGB_BUILTIN, 0, 0, 255);  // Blue
    break;

    case 4:
      Serial.println("Descending movement \n");
      rgbLedWrite(RGB_BUILTIN, 255, 255, 0);  // Yellow
    break;

    case 5:
      Serial.println("Circle anti clock wise \n");
      rgbLedWrite(RGB_BUILTIN, 255, 255, 255);  // White
      delay(100);
      rgbLedWrite(RGB_BUILTIN, 0, 0, 0);  // Off
      delay(100);
      rgbLedWrite(RGB_BUILTIN, 255, 255, 255);  // White
    break;

    case 6:
      Serial.println("Circle clock wise : Fix white \n");
      rgbLedWrite(RGB_BUILTIN, 255, 255, 255);  // White

    break;


    default:
      Serial.println("Problem !\n");
      break;

  }

}
