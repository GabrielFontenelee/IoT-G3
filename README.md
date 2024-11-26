Travail en groupe à l'École Centrale de Lille

4. Utilisation d’un noeud capteur pour tracker les mouvements d’un autre noeud
Use an Arduino Nano 33 sense to capture different movements :  
for right movement : Blue light;
For left movement : Green light;
For ascending movement: Red light;
For descending movement: Yellow light
Circle (clock orientation) :  Blinking white
Circle (clock orientation) : Fix white
Send that information to another node (ESP 32)  by using BLE beacons. The receiver node should turn on the corresponding light and publish the identified movement through a web server (MQTT).

# Running the project

1. Install mosquitto

2. Run mosquitto
```sh
mosquitto -v -c test.conf
```

3. Get your IP 
```sh 
ifconfig
```

4. (Optional) test MQTT server
Terminal 1: `mosquitto -v -c test.conf`
Terminal 2: `mosquitto_sub -h $IP -p 1883 -t test/topic`
Terminal 3: `mosquitto_pub -h 192.168.17.29 -t test/topic -m testpa`

Yout should see a message in Terminal 2 and logs in Terminal 1.

5. Upload MQTT to the esp32 and check that it connected to WiFi

6. Upload main_gyroscope to Arduino equipped with Giroscope

7. WEB


