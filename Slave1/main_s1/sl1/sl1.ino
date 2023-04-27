
#include "Arduino.h"
#include "LoRa_E32.h"
#include "DHT.h"



//Prototype
void handshaking();
void RevRequest();
void readSensors();
void RevMess();
boolean runEvery(unsigned long interval);


           
const int DHTPIN = 4;      //D4
const int DHTTYPE = DHT11;  
const int flamePin = 5;

LoRa_E32 e32ttl(3,2);
DHT dht(DHTPIN, DHTTYPE);

struct Message 
{
  byte ID = 1;
  byte addl = 0x02;
  byte addh = 0x00;
  byte temp[2];
  byte humi[2];
  byte mq2Val[2];
  byte flameVal[2];
  byte connectingToMaster[4] = {0,0,0,0};
  byte flag_handshaking = 0;
} message;

int idOfFather;
byte addlOfFather;
byte addhOfFather;

int firstRequest = 1;

byte ACK = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  e32ttl.begin();
  dht.begin(); 
  pinMode(5, INPUT_PULLUP); 
}



void loop() {
  handshaking(); 
  while(1)
  {
    RevRequest();
  }
}



void handshaking() {
  while(runEvery(20000) == 0) {
    if (e32ttl.available() > 1) {
      ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
      struct Message rxACK = *(Message*)rsc.data;
      Serial.println(rxACK.ID);
      if (rxACK.ID == 0) {
        ResponseStatus rs = e32ttl.sendFixedMessage(0x01, 0x01, 0x03, &message, sizeof(Message));
      }    
      free(rsc.data);
    }
  }      
}



void RevRequest(){
  if (e32ttl.available() > 1) {
    ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
    struct Message rxMess = *(Message*)rsc.data;
    Serial.print("receive request from ");
    Serial.println(rxMess.ID);
    if (firstRequest == 1) {
      firstRequest = 0;
      idOfFather = rxMess.ID;
      addlOfFather = rxMess.addl;
      addhOfFather = rxMess.addh;
    }
    
    if (rxMess.ID == idOfFather) {
      for (int i = 0; i < 4; i++)
        message.connectingToMaster[i] = rxMess.connectingToMaster[i];
      for (int i = 1; i <= 4; i++) {
        Serial.print(message.connectingToMaster[i-1]); 
      }
      readSensors();
      delay(500);
      ResponseStatus rs = e32ttl.sendFixedMessage(addhOfFather, addlOfFather, 0x03, &message, sizeof(Message));
      Serial.println("--");
      for (int i = 1; i <= 4; i++) {
        if ( (message.connectingToMaster[i-1] == 0) && (message.ID != i) && (idOfFather != i) ) {
          ResponseStatus rs1 = e32ttl.sendFixedMessage(0x00, i+1, 0x03, &message, sizeof(Message));
          int cnt=0;
          while (cnt != 3000) {
            cnt++;
            RevMess();
            delay(10);  
          }
//          while(runEvery(40000) == 0) {
//            RevMess();
//          }
        }
      } 
    }
 
    free(rsc.data);
  }
}



void readSensors() {
  *(short*)message.temp     = dht.readTemperature(); 
  *(short*)message.humi     = dht.readHumidity(); 
  *(short*)message.mq2Val   = analogRead(A0); 
  *(short*)message.flameVal = digitalRead(5);
  Serial.print("Temp: ");
  Serial.println(*(short*)(message.temp));               
  Serial.print("Humi: ");
  Serial.println(*(short*)message.humi);
  Serial.print("MQ-2: ");
  Serial.println(*(short*)message.mq2Val);
  Serial.print("Digital Value FL: ");
  Serial.println( *(short*)message.flameVal);
}



void RevMess(){
  if (e32ttl.available() > 1) {
    ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
    struct Message rxMess = *(Message*)rsc.data;
    if (rxMess.ID != message.ID && rxMess.ID != idOfFather)
    {
    //nhan duoc tu slave va in ra
      Serial.print("receive mess from ");
      Serial.println(rxMess.ID);
      Serial.print("Temp: ");
      Serial.println(*(short*)(rxMess.temp));               
      Serial.print("Humi: ");
      Serial.println(*(short*)rxMess.humi);
      Serial.print("MQ-2: ");
      Serial.println(*(short*)rxMess.mq2Val);
      Serial.print("Digital Value FL: ");
      Serial.println( *(short*)rxMess.flameVal);
      //if (rxMess.ID >= 1 && rxMess.ID <= 4)
      delay(100);
      Serial.println( addhOfFather);
      Serial.println( addlOfFather);
      ResponseStatus rs = e32ttl.sendFixedMessage(addhOfFather, addlOfFather, 0x03, &rxMess, sizeof(Message)); 
      delay(200);
      rs = e32ttl.sendFixedMessage(addhOfFather, addlOfFather, 0x03, &rxMess, sizeof(Message)); 
      delay(300);
      free(rsc.data);
    }
  }
}



boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
