#include <SoftSerial.h>

uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x04, 0xFB, 0x12};
uint8_t set_baudrate[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};
int irData = 0;   // for incoming serial data
int BluetoothData = 0; // the data given from Computer
int LED_PIN = 1;
int IR_TX_PIN = 0;
int IR_RX_PIN = 2;
int BT_TX_PIN = 3;
int BT_RX_PIN = 4;
int BUTTON_PIN = 5;

SoftSerial ir(IR_RX_PIN, IR_TX_PIN); // RX, TX
SoftSerial BT(BT_RX_PIN, BT_TX_PIN); // RX, TX

// the setup routine runs once when you press reset:
void setup() {
        statusblink();
        ir.begin(9600);
        BT.begin(9600);
        pinMode(LED_PIN, OUTPUT); //LED on Model A 
        pinMode(BUTTON_PIN, INPUT);  
        ir.write(set_baudrate,sizeof(my_serial_bytes));
        statusblink();
}


void loop() {
  
  //digitalWrite(LED_PIN, HIGH);
  //ir.write(my_serial_bytes,sizeof(my_serial_bytes));
  //digitalWrite(LED_PIN, LOW); 
  //delay(1000);
  if (digitalRead(BUTTON_PIN) > 0) {
    digitalWrite(LED_PIN, HIGH);
    ir.write(my_serial_bytes,sizeof(my_serial_bytes));
    digitalWrite(LED_PIN, LOW);
  }
  //else  digitalWrite(LED_PIN, LOW);
  read_ir();
  delay(1000);
  read_bt();
  delay(1000);
}


//  BT.println("Bluetooth On please press 1 or 0 blink LED ..");

void statusblink() {
  digitalWrite(LED_PIN, HIGH);
  delay(10);        
  digitalWrite(LED_PIN, LOW); 
  delay(200); 
  digitalWrite(LED_PIN, HIGH);
  delay(10);        
  digitalWrite(LED_PIN, LOW); 
  delay(200); 
}
void read_ir() {
  while (ir.available() > 0) {
        irData = ir.read();
        statusblink();  
  }
}

void read_bt() {
  while (BT.available() > 0) {
    BluetoothData=BT.read();
    if (BluetoothData='1') ir.write(my_serial_bytes,sizeof(my_serial_bytes));
    statusblink();  
  }
}
