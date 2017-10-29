/*
#define CAN_125KBPS  13
#define CAN_200KBPS  14
#define CAN_250KBPS  15
#define CAN_500KBPS  16
*/
#include <SPI.h>
#include "mcp_can.h"

//const int SPI_SCK_PIN = 13;
//const int SPI_MISO_PIN = 12;
//const int SPI_MOSI_PIN = 11;
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin
//const int SPI_INT_PIN = 2;

#define CAN_ID 0x00
const int InteruptPin = 2;


// send data:  id = 0x00, standard frame, data len = 8, stmp[8] = {a, b, c, d, e, f, g, h}: data buf
// CAN.sendMsgBuf(0x00,0, 8, stmp);



char  MsgBuff[200];
int indexBuff = 0;

unsigned char len = 0;
unsigned char buf[8];

const int analogPin = 0;

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
    
    attachInterrupt(digitalPinToInterrupt(InteruptPin), CanRead, FALLING);
    interrupts();
    //pinMode(13,OUTPUT);
    //digitalWrite(13,HIGH);
    
}


void loop()
{
    /*
    double volt = ((double) analogRead(analogPin))*5/1023;
    String  msg = String(volt,4);
    char sendmsg[msg.length()];
    msg.toCharArray(sendmsg,msg.length());
    Serial.print(sendmsg);
    Serial.println(" V");
    Serial.println(sizeof(sendmsg));
    CanWrite(sendmsg);
    delay(500);
    */
      
    if(indexBuff > 6) {
      xbeeSend(indexBuff);
    }
    
}

void CanWrite(char msg[]) {
  CAN.sendMsgBuf(0x01,0,sizeof(msg)+4, msg);
  
}



void CanRead() {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned char canId = CAN.getCanId();
        String nameCanId = nameSender(canId);
        int nameLength = nameCanId.length();
        char Idname[nameLength+1];
        nameCanId.toCharArray(Idname, nameLength+1);
        // add message name
        for(int i = 0; i < nameLength; i++) {
          MsgBuff[i+indexBuff] = Idname[i];
        }
        indexBuff = indexBuff + nameLength;
        // add message
        for(int j = 0; j < len; j++) {
          MsgBuff[j+indexBuff] = (char) buf[j];
        }
        indexBuff = indexBuff + len;
        /*
        Serial.println("-----------------------------");
        Serial.println("get data from ID: ");
        Serial.println(canId);

        for(int i = 0; i<len; i++)    // print the data
        {
            Serial.print(buf[i]);
            Serial.print("\t");
        }
        Serial.println();
        */
}

// send message of length Msglength over Xbee
void xbeeSend(int Msglength) {
  noInterrupts();
  /*
  for(int i = 0; i <= indexBuff; i++) {
    Serial.print(MsgBuff[i]);
  }
  indexBuff = 0;
  */
  // send message of length Msglength over serial (Xbee)
  for(int i = 0; i < Msglength; i++) {
    Serial.print(MsgBuff[i]);
  }
  // Delete sent message from buffer
  for(int j = 0; j < Msglength; j++) {
    MsgBuff[j] = MsgBuff[j+Msglength];
  }
  indexBuff = indexBuff-Msglength;
  Serial.println();
  interrupts();
}


String nameSender(unsigned char canID) {
  switch(canID) {
    case 0x01:
      return "Volt/X";
    case 0x02:
      return "Ohm /X";
    case 0x03:
      return "Amp /X";
    default:
      return "NA  /X";
  }
}



