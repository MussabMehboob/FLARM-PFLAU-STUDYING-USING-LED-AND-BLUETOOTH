#include"esp32-hal-uart.h"
#include"PFLAUSentenceDecoding.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#define RXD2 16
#define TXD2 17
BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;                   //Variables for bluetooth connection
bool oldDeviceConnected = false;
// https://www.uuidgenerator.net/
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9"
//unsigned long baud=0
uart_t* uart2;                            //uart object to detect baud rate
unsigned long baud;                       //Baud Variable
bool BluetoothEnable=false;               //Boolean to check if bluetooth is enable or not
const int BluetoothPin=18;
const int LedPin=25;                      //LEDPin and PowerSetting Pins
const int PowerSetting1=19;         
const int PowerSetting2=21;
int PowerSetting=0;                //By default 0=Low Power Setting ,1=Mid Power Setting , 2= High Power Setting
int AlarmRaiseTime=0;
String NMEADATA;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void FLASHLOWPOWER(int Alarm=4){  //If alarm ==4 this means power on flash
digitalWrite(LedPin,HIGH);     //: 35 ms on - 50 ms off - 35 ms on - 2200 ms off - 35 ms on - ... (Alarmlevel 0: 1800 ms instead of 2200 ms long pause, level 1: 1500 ms; 2: 1100 ms; 3; 800 ms)
delay(35);
digitalWrite(LedPin,LOW);
delay(50);
digitalWrite(LedPin,HIGH);
delay(35);
digitalWrite(LedPin,LOW);
delay(2200);                  //Flashing to confirm the power settings are right
digitalWrite(LedPin,HIGH) ;      
delay(35);
digitalWrite(LedPin,LOW);
if(Alarm==4)
{
delay(2200);                          //This condition is satisfied only when powering on
}
if(Alarm==1)
{
delay(1500);
}               //Judging the Alarm Level
if(Alarm==2)
{
delay(1100);
}
if(Alarm==3)      //Adding delay accoding to the alarm level
{
delay(800);
}
}
void FLASHMIDPOWER(int Alarm=4){            //If alarm ==4 this means power on flash
digitalWrite(LedPin,HIGH);
delay(35);
digitalWrite(LedPin,LOW);
delay(50);                       // Mid power setting: 35 ms on – 50 ms off – 35 ms on – 50 ms off – 35 ms on – 2200 ms off –35 ms on -                              
digitalWrite(LedPin,HIGH);
delay(35);  
digitalWrite(LedPin,LOW);
delay(50);
digitalWrite(LedPin,HIGH);
delay(35);    
digitalWrite(LedPin,LOW);
delay(2200);                  //Flashing to confirm the power settings are right
digitalWrite(LedPin,HIGH);       
delay(35);    
digitalWrite(LedPin,LOW);
if(Alarm==0)
{
delay(1800);
}
if(Alarm==4)
{

delay(2200);                          //This condition is satisfied only when powering on
}
if(Alarm==1)
{
delay(1500);
}               //Judging the Alarm Level
if(Alarm==2)
{
delay(1100);
}
if(Alarm==3)      //Adding delay accoding to the alarm level
{
delay(800);
}
}
void FLASHHIGHPOWER(int Alarm=4){  //If alarm ==4 this means power on flash
digitalWrite(LedPin,HIGH);
delay(35);
digitalWrite(LedPin,LOW);
delay(50);
digitalWrite(LedPin,HIGH);
delay(35);
digitalWrite(LedPin,LOW);
delay(50);
digitalWrite(LedPin,HIGH);
delay(35); 
digitalWrite(LedPin,LOW);
delay(1500);
digitalWrite(LedPin,HIGH);
delay(35);
digitalWrite(LedPin,LOW);
if(Alarm==4)
{
  delay(1500);
}
if(Alarm==0)
{
delay(1100);
}
if(Alarm==1)
{
delay(900);
}               //Judging the Alarm Level
if(Alarm==2)
{
delay(700);
}
if(Alarm==3)      //Adding delay accoding to the alarm level
{
delay(500);
}
  }
void POWERON(){
for(int i=0;i<5;i++)
{
if(PowerSetting==0)
{
FLASHLOWPOWER();
}
if(PowerSetting==1)
{
  FLASHMIDPOWER();
}
if(PowerSetting==2)
{
 FLASHHIGHPOWER();
}
} 
}

void setup() {
   Serial.begin(115200);
pinMode(BluetoothPin, INPUT);         //Pin to take statue of bluetooth
pinMode(LedPin,OUTPUT);                             //Output Pin for LED  
pinMode(PowerSetting1,INPUT);         
pinMode(PowerSetting2,INPUT);           //Power Mode Pins 

BluetoothEnable=digitalRead(BluetoothPin);
if(BluetoothEnable)
{
     // Create the BLE Device
  BLEDevice::init("UART Service");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                   CHARACTERISTIC_UUID_TX,
                    BLECharacteristic::PROPERTY_NOTIFY
                  );
                      
  pTxCharacteristic->addDescriptor(new BLE2902());
  // Start the service
  pService->start();
// Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");   //Turn on the bluetooth 
}

if(digitalRead(PowerSetting1)==HIGH&&digitalRead(PowerSetting2)==HIGH)
{
PowerSetting=2;
}

if(digitalRead(PowerSetting1)==HIGH||digitalRead(PowerSetting2)==HIGH)
{
PowerSetting=1;
}
else
{
  PowerSetting=0;
}
uart2=uartBegin(1,9600,SERIAL_8N1, RXD2, TXD2,256,false);
while(true)
{
baud=uartDetectBaudrate(uart2);
if(baud!=0)
{Serial.print("Detected Baud RATE:");
  Serial.println(baud);
uartEnd(uart2, RXD2, TXD2);
break;
}}
Serial2.begin(baud);
POWERON();
Serial.print("Power on routine is done");

}
void loop(){

PFLAUSentenceDecoding MYSENTENCE=PFLAUSentenceDecoding(NMEADATA);       ///Making an object of our class to get data from PFLAU and store it in different variables
if(BluetoothEnable)
{
  if (deviceConnected) {
    String pflau=MYSENTENCE.c;
        int str_len = pflau.length() + 1; 
// Prepare the character array (the buffer) 
      char char_array[str_len];
        // Copy it over 
        pflau.toCharArray(char_array, str_len);
        pTxCharacteristic->setValue(char_array);
        pTxCharacteristic->notify();
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
        }
}
}

while(Serial.available()==0)
{       
  if(MYSENTENCE.GPS==1)
  {
    digitalWrite(LedPin,HIGH);                                //if on ground the led will simply not flash
  }
  //WHen there is no serial data available then flash accroding to previous settings 
if(MYSENTENCE.GPS==2)
{
  if(PowerSetting==0)
  {
  FLASHLOWPOWER(MYSENTENCE.ALARMLEVEL);             //LOW POWER Flashing according to alarm level
    
  }
 if(PowerSetting==1)
  {
 FLASHMIDPOWER(MYSENTENCE.ALARMLEVEL);                //MID POWER Flashing according to alarm level
  }
 if(PowerSetting==2)
  {
 FLASHHIGHPOWER(MYSENTENCE.ALARMLEVEL);               //HIGH POWER Flashing according to alarm level
  }

}
}
while(Serial.available()>0&&BluetoothEnable==false)
{
  NMEADATA=Serial.readString();

}

}
