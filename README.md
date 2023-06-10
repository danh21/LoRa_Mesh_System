# Urban-monitoring-system-using-Lora-Mesh-network
This system includes 4 Slave(using Arduino Nano) and 1 Master(using ESP8266)
About Master:
- It will send connection request to the Slave and then waiting respond from the Slave.
- After receiving Slave's respond. It will store Slave's address to communicate.
About all Slave, it will wait the request from Master. After receiving request, it will collect enviromental value then it will caculate and send that data to Master. 
- Those Slaves will scan to other Slaves which didn't come to connect with the Master and wait to receive data from it.
- After that, it will send that data to Master(this is the immediate Slave, it can send other Slave's data to Master).
