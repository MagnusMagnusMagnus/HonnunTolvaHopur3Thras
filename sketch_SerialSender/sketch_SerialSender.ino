/*
char  MsgBuff[200];
int indexBuff = 0;
*/
int analogPin = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

  double volt = ((double) analogRead(analogPin))*5/1023;
  Serial.print(volt);
  Serial.print(" V");
  delay(1000);
  /*
  if(Serial.available()) {
    readXbee();
  }
  if(indexBuff > 15)
    printMsg();
   */
}

/*
void readXbee() {
  while(Serial.available()) {
    MsgBuff[indexBuff] = char(Serial.read());
    indexBuff++;
  }
}

void printMsg() {
  for(int i = 0; i <= indexBuff; i++) {
    Serial.print(MsgBuff[i]);
  }
  indexBuff = 0;
}
*/
