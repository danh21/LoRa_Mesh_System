
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "Arduino.h"
#include "LoRa_E32.h"
#include "DHT.h"
#include "icon.h"
#include <string.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>


const char* ssid ="Lau 1_2";
const char* pass ="A1234567";

// Insert Firebase project API Key
#define FIREBASE_HOST "https://datn-lora-default-rtdb.firebaseio.com/"
// Insert RTDB URLefine the RTDB URL */
#define FIREBASE_AUTH "d9X1K9HuQRJUwWz7cJ1PQCSrZJ9EHnY1PbDbktpl"

//Define Firebase Data object
FirebaseData fbdb;

//Prototype
void LCD_Init();
void printText(char *text, uint16_t color, int x, int y,int textSize );
void printValue();
void clearValue();
void updateValue(int x);
void handshaking();
void RevACK();
void RevMess();
boolean runEvery(unsigned long interval);

//------LCD TFT------ESP8266--------------------
#define TFT_CS         16   //D0
#define TFT_RST        5    //D1                                     
#define TFT_DC         4    //D2
#define TFT_MOSI       14   //D5
#define TFT_SCLK       12   //D6

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//------------define colors----------------
#define BLACK          0x0000    
#define BLUE           0x001F    
#define RED            0xF800      
#define GREEN          0x07E0
#define CYAN           0x07FF
#define MAGENTA        0xF81F  
#define YELLOW         0xFFE0
#define WHITE          0xFFFF 



//LoRa_E32 e32ttl(D4,D3);                 //init object for LoRa E32 with pin D4,D3 on ESP8266
LoRa_E32 e32ttl(2,0);



struct Message 
{
  //char ID[7]="Master";
  byte ID = 0;
  byte addl = 0x01;
  byte addh = 0x01;
  byte temp[2];
  byte humi[2];
  byte mq2Val[2];
  byte flameVal[2];
  byte connectingToMaster[4] = {0,0,0,0};
  byte flag_handshaking = 0;
} message;

int isACK = 0;
String temp_,humi_,gas_,flame_;
int complete=0;

int sl3=0, sl4=0;



// Initialize WiFi
void initWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  e32ttl.begin();
  tft.initR(INITR_BLACKTAB);                      
  tft.fillScreen(BLACK);                 //background
  LCD_Init();
  
  initWiFi();
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}


void loop()
{
  handshaking();
  while(1)
  {
    for (int i = 1; i <= 4; i++) {
      Serial.print(message.connectingToMaster[i-1]); 
    }
    Serial.println();
    for (int i = 1; i <= 4; i++) {
      if (message.connectingToMaster[i-1] == 1) {
        ResponseStatus rs = e32ttl.sendFixedMessage(0x00, i+1, 0x03, &message, sizeof(Message));  //send data request package 
        while(runEvery(60000) == 0) {
          RevMess();
        }
      }
    }
  } 
}


void handshaking() {
  for (int addr = 2; addr <= 5; addr++) {
    ResponseStatus rs = e32ttl.sendFixedMessage(0x00, addr, 0x03, &message, sizeof(Message));
    delay(1000);  
    rs = e32ttl.sendFixedMessage(0x00, addr, 0x03, &message, sizeof(Message)); 
    delay(200);   
    while(runEvery(10000) == 0) {
      RevACK();
      if (isACK) {
        isACK = 0;
        break;
      }
      delay(200);
    }     
  } 
}

void RevACK(){
  if (e32ttl.available() > 1) {
    ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
    struct Message rxACK = *(Message*)rsc.data;
    Serial.println(rxACK.ID);     
    if (rxACK.ID >= 1 && rxACK.ID <= 4) {
      message.connectingToMaster[rxACK.ID - 1] = 1;
      isACK = 1;   
    } 
    free(rsc.data);
  }
}



void RevMess() {
  if (e32ttl.available()  > 1) {
    ResponseStructContainer rsc = e32ttl.receiveMessage(sizeof(Message));
    struct Message rxMess = *(Message*)rsc.data;
    Serial.println(rxMess.ID);
    Serial.println(*(short*)(rxMess.temp));
    Serial.println(*(short*)(rxMess.humi));
    Serial.println(*(short*)(rxMess.mq2Val));
    Serial.println(*(short*)(rxMess.flameVal));
    
    int temp  = *(short*)rxMess.temp;
    int humi  = *(short*)rxMess.humi;
    int gas   = *(short*)rxMess.mq2Val;
    int flame = *(short*)rxMess.flameVal;
          
    temp_ = String(temp);
    humi_ = String(humi);
    gas_  = String(gas);
    flame_= String(flame);
  String id="";
    if (rxMess.ID == 1)
    {    
      updateValue(32); 
       id = "Slave 1";
    }
    else if (rxMess.ID == 2)
    {
      updateValue(64);  
       id = "Slave 2";
    }
    else if (rxMess.ID == 3)
    {
      updateValue(96);    
       id = "Slave 3";
    } 
    else if (rxMess.ID == 4)
    {
      updateValue(128);
       id = "Slave 4";
    }
    
    Firebase.setInt(fbdb, id + "/temp",temp_.toInt());
    Firebase.setInt(fbdb, id + "/humi",humi_.toInt());
    Firebase.setInt(fbdb, id + "/gas",gas_.toInt());
    Firebase.setInt(fbdb, id + "/flame",flame_.toInt());
    delay(200);
    free(rsc.data);

  }
}



void LCD_Init() 
{
  // vertical screen, create 5 lines
  tft.drawLine(0,   0, 128,   0, CYAN);
  tft.drawLine(0,  32, 128,  32, CYAN);
  tft.drawLine(0,  64, 128,  64, CYAN);
  tft.drawLine(0,  96, 128,  96, CYAN);
  tft.drawLine(0, 128, 128, 128, CYAN);
  
  tft.setRotation(3);                             //flip -> horizontal screen 
  // horizontal screen, create 5 lines
  tft.drawLine(0,  25, 160,  25, CYAN);
  tft.drawLine(0,  50, 160,  50, CYAN);
  tft.drawLine(0,  75, 160,  75, CYAN);
  tft.drawLine(0, 100, 160, 100, CYAN);
  tft.drawLine(0, 127, 160, 127, CYAN);

  tft.drawRGBBitmap(0+8,  25+4,  temp, 20, 20);      //icon temp
  tft.drawRGBBitmap(0+8,  50+4,  humi, 20, 20);      //icon humidity
  tft.drawRGBBitmap(0+8,  75+4,   gas, 20, 20);      //icon gas
  tft.drawRGBBitmap(0+8, 100+4, flame, 20, 20);      //icon flame
   
  printText("S1", GREEN,  32+5, 8, 2);
  printText("S2", GREEN,  64+5, 8, 2);
  printText("S3", GREEN,  96+5, 8, 2);
  printText("S4", GREEN, 128+5, 8, 2);
}

void printText(char *text, uint16_t color, int x, int y,int textSize ){
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.print(text);
}

void printValue(String value, int x, int y){
  tft.setCursor(x, y);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println(value);
}

void clearValue(String text,int x, int y){
  tft.setCursor(x, y);
  tft.setTextColor(WHITE,BLACK);
  tft.setTextSize(1);
  tft.print(text);
}

void updateValue(int x) {
  clearValue( temp_, x+5, 25+7 );
  clearValue( humi_, x+5, 50+7 );
  clearValue(  gas_, x+5, 75+7 );
  clearValue(flame_, x+5, 100+7);
  
  clearValue("  ", x+15, 25+7 );
  clearValue("  ", x+15, 50+7 );
  clearValue("  ", x+15, 75+7 );
  clearValue("  ", x+15, 100+7);
  
  printValue( temp_, x+5, 25+7 );
  printValue( humi_, x+5, 50+7 );
  printValue(  gas_, x+5, 75+7 );
  printValue(flame_, x+5, 100+7); 
}

//void Updatedata_fb()
//{
//  Firebase.setInt(fbdb, formattedDate + "/temp",temp_.toInt());
//  Firebase.setFloat(fbdb, formattedDate + "/humi",humi_.toInt());
//  Firebase.setFloat(fbdb, formattedDate + "/gas",gas_.toInt());
//  Firebase.setFloat(fbdb, formattedDate + "/flame",flame_.toInt());
//}


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
