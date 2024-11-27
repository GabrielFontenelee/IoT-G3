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

3. Get your IP address
```sh 
ifconfig |  grep 192.168 | cut --delimiter ' ' --fields 10
```

4. Test MQTT server
Terminal 1: `mosquitto -v -c test.conf`
Terminal 2: `mosquitto_sub -h $IP -p 1883 -t esp32-001/send`
Terminal 3: `mosquitto_pub -h $IP -t esp32-001/send -m testpa -i terminal`

You should see a message in Terminal 2 and logs in Terminal 1.

5. Upload MQTT to the esp32 and check that it connected to WiFi

Change partition size to 8MB

Temporarilly disable your firewall for it to connect or add an exception for Mosquitto on port 1883.

6. Upload main_gyroscope to Arduino equipped with Giroscope

7. Upload the BLE server to another ESP32

8. Connect the the two ESP32s 

In RXTX 16 17 and GND

9. WEB

Add another exception on the firewall

