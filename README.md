# Urban-monitoring-system-using-Lora-Mesh-network
This system includes 4 Slaves (using Arduino Nano as main microcontroller) and 1 Master (using ESP8266 as main microcontroller)

About Master:
- It will send connection requests to all Slaves and then waiting respond from Slaves.
- After receiving respond of Slaves, it will store addresses of connected Slaves to communicate.

About all Slaves:
- It will wait the request from Master. After receiving request, it will collect environmental values, then it will calculate and send that data to Master. 
- It will scan to other Slaves which didn't connect with the Master before and wait to receive data from it.
- After that, it will send that data to Master (this is the mediate Slave, it can send data of other Slaves to Master).
