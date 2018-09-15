#include <stdlib.h>

uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x04, 0xFB, 0x12};
uint8_t set_baudrate[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};
int ir_data = 0,   // for incoming serial data
    bt_data = 0, // the data given from Computer
    ir_tx_pin = 0,
    led_pin = 1,
    ir_rx_pin = 2,
    bt_tx_pin = 3,
    bt_rx_pin = 4,
    button_pin = 5;
    
unsigned long check_interval_trigger_in_ms = 50,
              check_interval_bt_in_ms = 100,
              check_interval_ir_in_ms = 100;
              

//Serial ir(ir_rx_pin, ir_tx_pin); // RX, TX
//Serial bt(bt_rx_pin, bt_tx_pin); // RX, TX
typedef enum {READY, STREAM} state_t ;

state_t state = READY;
// the setup routine runs once when you press reset:
void setup() {
        Serial.begin(115200);
        statusblink();
        ir.begin(9600);
        bt.begin(9600);
        pinMode(led_pin, OUTPUT); //LED on Model A 
        pinMode(button_pin, INPUT);  
        ir.write(set_baudrate,sizeof(my_serial_bytes));
        statusblink();
}


void loop() {
  unsigned long time_in_ms = 0;


  switch(state) {
    case READY:
      time_in_ms = millis();
      check_trigger(time_in_ms, check_interval_trigger_in_ms);
      check_bt(time_in_ms, check_interval_bt_in_ms);
      stream_ir_to_bt(time_in_ms, check_interval_ir_in_ms);
      break;

    case STREAM:
      time_in_ms = millis();
      check_trigger(time_in_ms, check_interval_trigger_in_ms);
      stream_bt_to_ir(time_in_ms, check_interval_bt_in_ms);
      stream_ir_to_bt(time_in_ms, check_interval_ir_in_ms);
      break;
  }
  
  //digitalWrite(led_pin, HIGH);
  //ir.write(my_serial_bytes,sizeof(my_serial_bytes));
  //digitalWrite(led_pin, LOW); 
  //delay(1000);
  if (digitalRead(button_pin) > 0) {
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


//  bt.println("Bluetooth On please press 1 or 0 blink LED ..");

void statusblink() {
  digitalWrite(led_pin, HIGH);
  delay(10);        
  digitalWrite(led_pin, LOW); 
  delay(200); 
  digitalWrite(led_pin, HIGH);
  delay(10);        
  digitalWrite(led_pin, LOW); 
  delay(200); 
}
void read_ir() {
  while (ir.available() > 0) {
        ir_data = ir.read();
        statusblink();  
  }
}

void read_bt() {
  while (bt.available() > 0) {
    bt_data=bt.read();
    if (bt_data='1') ir.write(my_serial_bytes,sizeof(my_serial_bytes));
    statusblink();  
  }
}
