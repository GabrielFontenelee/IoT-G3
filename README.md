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
