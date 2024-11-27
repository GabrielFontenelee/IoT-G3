#include <Arduino_LSM9DS1.h>

#define DT 10

float x_a, y_a, z_a;
float xprev_a = 0, yprev_a = 0, zprev_a = 0;
float x_g, y_g, z_g;
float xprev_g = 0, yprev_g = 0, zprev_g = 0;

float offset_a [3] = {0,0,0}; // ax, ay, az 
float offset_g [3] = {0, 0, 0}; // gx, gy, gz

const float delta_gyro = 5;
const float delta_acce = 0.05;

uint8_t movement; 

void calibrateSensor();
unsigned long waitNextPeriod();

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

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in g's");
  Serial.println("X\tY\tZ");

  calibrateSensor();

}

void loop() {
  // put your main code here, to run repeatedly:

  waitNextPeriod();

  bool moveRight = false;
  bool moveLeft = false;
  bool moveUp = false;
  bool moveDown = false;
  bool rotateCW = false;
  bool rotateCCW = false;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x_a, y_a, z_a);

    // Subtrair offsets
    x_a -= offset_a[0];
    y_a -= offset_a[1];
    z_a -= offset_a[2];

    float dx_a = x_a - xprev_a;
    float dy_a = y_a - yprev_a;
    float dz_a = z_a - zprev_a;

    // Atualiza 
    xprev_a = x_a;
    yprev_a = y_a;
    zprev_a = z_a;

    // Detecção de movimentos
    if (dy_a > delta_acce ) {
      moveRight = true;
      movement = 0x01;
    } else if (dy_a < -delta_acce) {
      moveLeft = true;
      movement = 0x02;
    }

    if (dx_a > delta_acce) {
      moveDown = true;
      movement = 0x03;
    } else if (dx_a < -delta_acce) {
      moveUp = true;
      movement = 0x04;
    }

    // Imprimir valores para depuração
   /*
    Serial.print("Aceleração: x=");
    Serial.print(x_a);
    Serial.print(", y=");
    Serial.print(y_a);
    Serial.print(", z=");
    Serial.println(z_a);
  */
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x_g, y_g, z_g);

    float dx_g = x_g - xprev_g;
    float dy_g = y_g - yprev_g;
    float dz_g = z_g - zprev_g;

    // Atualiza 
    xprev_g = x_g;
    yprev_g = y_g;
    zprev_g = z_g;


    if (dz_g > delta_gyro) {
      rotateCCW = true;
      movement = 0x05;
    } else if (dz_g < -delta_gyro) {
      rotateCW = true;
      movement = 0x06;
    }

    // Imprimir valores para depuração
    /*
    Serial.print("Giroscópio: x_g=");
    Serial.print(x_g);
    Serial.print(", y_g=");
    Serial.print(y_g);
    Serial.print(", z_g=");
    Serial.println(z_g);
    */
  }

  // Processamento dos movimentos
  if (moveRight) {
    Serial.println("Movimento para a direita detectado");
    // Acender luz azul
  }
  if (moveLeft) {
    Serial.println("Movimento para a esquerda detectado");
    // Acender luz verde
  }
  if (moveUp) {
    Serial.println("Movimento ascendente detectado");
    // Acender luz vermelha
  }
  if (moveDown) {
    Serial.println("Movimento descendente detectado");
    // Acender luz amarela
  }
  if (rotateCW) {
    Serial.println("Movimento circular horário detectado");
    // Acender luz branca fixa
  }
  if (rotateCCW) {
    Serial.println("Movimento circular anti-horário detectado");
    // Acender luz branca piscando
  }

  Serial.println(movement);

  delay(100);

}

void calibrateSensor() {
  const int calibrationSamples = 100;
  for (int i = 0; i < calibrationSamples; i++) {
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x_g, y_g, z_g);
      offset_g[0] += x_g;
      offset_g[1] += y_g;
      offset_g[2] += z_g;
    }
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x_a, y_a, z_a);
      offset_a[0] += x_a;
      offset_a[1] += y_a;
      offset_a[2] += z_a;
    }
    delay(10);
  }
  offset_a[0] /= calibrationSamples;
  offset_a[1] /= calibrationSamples;
  offset_a[2] /= calibrationSamples;

  offset_g[0] /= calibrationSamples;
  offset_g[1] /= calibrationSamples;
  offset_g[2] /= calibrationSamples;

  Serial.println("Calibração concluída");
  Serial.print("Offsets Accelerometre: x=");
  Serial.print(offset_a[0]);
  Serial.print(", y=");
  Serial.print(offset_a[1]);
  Serial.print(", z=");
  Serial.println(offset_a[2]);

  Serial.print("Offsets Gyroscope: x=");
  Serial.print(offset_g[0]);
  Serial.print(", y=");
  Serial.print(offset_g[1]);
  Serial.print(", z=");
  Serial.println(offset_g[2]);
}

unsigned long waitNextPeriod() {
  static unsigned long last_time = 0;
  unsigned long current_time = millis();
  while ((current_time - last_time) < DT) {
    current_time = millis();
  }
  last_time = current_time;
  return current_time;
}

