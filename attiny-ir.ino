#include <SoftwareSerial.h>

uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x04, 0xFB, 0x12};
uint8_t set_baudrate[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};
int incomingByte = 0;   // for incoming serial data

SoftwareSerial mySerial(2, 0); // RX, TX

// the setup routine runs once when you press reset:
void setup() {
        statusblink();
        mySerial.begin(9600);
        pinMode(1, OUTPUT); //LED on Model A   
        mySerial.write(set_baudrate,sizeof(my_serial_bytes));
        statusblink();
}


void loop() {
  
  digitalWrite(1, HIGH);
  mySerial.write(my_serial_bytes,sizeof(my_serial_bytes));
  digitalWrite(1, LOW); 

  delay(1000);
  
  read_ir();

  delay(2000);

}

void statusblink() {
  digitalWrite(1, HIGH);
  delay(10);        
  digitalWrite(1, LOW); 
  delay(200); 
  digitalWrite(1, HIGH);
  delay(10);        
  digitalWrite(1, LOW); 
  delay(200); 
}
void read_ir() {
    while (mySerial.available() > 0) {
          // read the incoming byte:
          incomingByte = mySerial.read();
          statusblink();
  }
}
