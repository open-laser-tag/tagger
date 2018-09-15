#include "BluetoothSerial.h"
#include <pthread.h>

uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x04, 0xFB, 0x12};
uint8_t set_baudrate[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};
int ir_data = 0,   // for incoming serial data
    bt_data = 0, // the data given from Computer
    led_pin = 1,
    trigger_pin = 5;
    
unsigned long check_interval_trigger_in_ms = 50,
              check_interval_bt_in_ms = 100,
              check_interval_ir_in_ms = 100;
              
HardwareSerial ir(1); // rx_pin=3, tx_pin=1
HardwareSerial audio(2); //rx_pin=9, tx_pin=10
//HardwareSerial serial(3); //rx_pin=16, tx_pin=17
BluetoothSerial bt;

typedef enum {READY, STREAM} state_t ;

state_t state = READY;
// the setup routine runs once when you press reset:
void setup() {
        ir.begin(9600);
        bt.begin("ESP32");
        pinMode(trigger_pin, INPUT);  
        ir.write(set_baudrate,sizeof(my_serial_bytes));
        pthread_t threads[2];
}


void loop() {
  unsigned long time_in_ms = 0;


  switch(state) {
    case READY:
      time_in_ms = millis();
//        check_trigger(time_in_ms, check_interval_trigger_in_ms);
//        check_bt(time_in_ms, check_interval_bt_in_ms);
//        stream_ir_to_bt(time_in_ms, check_interval_ir_in_ms);
      break;

    case STREAM:
      time_in_ms = millis();
//      check_trigger(time_in_ms, check_interval_trigger_in_ms);
//      stream_bt_to_ir(time_in_ms, check_interval_bt_in_ms);
//      stream_ir_to_bt(time_in_ms, check_interval_ir_in_ms);
      break;
  }
  
  //digitalWrite(led_pin, HIGH);
  //ir.write(my_serial_bytes,sizeof(my_serial_bytes));
  //digitalWrite(led_pin, LOW); 
  //delay(1000);
  if (digitalRead(trigger_pin) > 0) {
    digitalWrite(led_pin, HIGH);
    ir.write(my_serial_bytes,sizeof(my_serial_bytes));
    digitalWrite(led_pin, LOW);
  }
  //else  digitalWrite(led_pin, LOW);
  read_ir();
  delay(1000);
  read_bt();
  delay(1000);
}


void read_ir() {
  while (ir.available() > 0) {
        ir_data = ir.read();
  }
}

void read_bt() {
  while (bt.available() > 0) {
    bt_data=bt.read();
    if (bt_data='1') ir.write(my_serial_bytes,sizeof(my_serial_bytes));
  }
}
