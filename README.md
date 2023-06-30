# Urban-monitoring-system-using-Lora-Mesh-network

# Description:
    - This system includes 4 Slaves (using Arduino Nano as main microcontroller) and 1 Master (using ESP8266 as main microcontroller).
    - About operation of Master:
        + First, Master handshakes to all Slaves in certain timeout and then wait responds from Slaves. If it receives ACK respond from Slave (connected Slave), it will store address of that Slave for polling.
        + Master polls connected Slaves to request those Slaves send their data, then it waits in certain timeout. If Master receives data from Slaves successfully, it will send data to Firebase to manage, then data is displayed on website interface.
    - About operation of Slave:
        + First, Slave waits handshaking from Master in certain timeout. After receiving handshaking, it sends ACK respond to accept connection.
        + If Slave receives request from Master, it will collect environmental values, calculate and send its data to Master. 
        + Slave scans to other Slaves which didn't connect to the Master before, then waits to receive data from those Slaves in certain timeout. If it receives data of other Slaves successfully, Slave will send that data to Master (this is the mediate Slave).

# Hardware:
    - ESP8266           (or other modules which support Wifi, UART, SPI standards)
    - Arduino Nano      (or other modules which support UART standard)
    - Module LoRaE32    (or other modules which support LoRa standard)
    - LCD TFT           (need to display symbols)
    - DHT11 sensor      (or other sensors which can measure temperature)
    - LM393 sensor      (or other sensors which can detect flame)
    - MQ-2 sensor       (or other sensors which can measure gas concentration)
    - Anten RF 433MHz   (or other antens which can connect to LoRa module)
    - Module MT3608     (or other modules which can increase voltage)
    - USB Tester        (optional, to measure power,...)

# Connection:
    More details at Principle_Diagram in Doc folder.

# Software:
    - Arduino IDE (programming for microcontroller).
    - Firebase (store data and location, you can create database yourself as Firebase_Interface in Doc folder, you have to edit rules to TRUE).
    - Web:
        + Link: https://danh21.github.io/Urban_Area_Monitoring_System_Using_LoRa_Mesh_Technology/
        + To monitoring locations, access to Locations Monitoring.

# Code:
    - Rev.ino (upload to Master):
        + Modify Wifi information
        + Modify link and authentication to your firebase
    - main.js
        + Config your firebase: Project settings (click on Gear Icon which is next to Project Overview) -> General -> Scroll down and copy your object firebaseConfig to replace.

# Bonus:
    - Block Diagram
    - Principle Diagram
    - Flowchart
    - Demo: https://youtu.be/a05Sg2Z3wNQ