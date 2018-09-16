#include "globals.h"
#include "BluetoothSerial.h"
#include <pthread.h>

uint8_t my_serial_bytes[5]={0xA1, 0xF1, 0x04, 0xFB, 0x12};
uint8_t set_baudrate_9600[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};
int ir_data = 0,   // for incoming serial data
    bt_data = 0; // the data given from Computer
    
HardwareSerial ir(1); // rx_pin=3, tx_pin=1
HardwareSerial audio(2); //rx_pin=9, tx_pin=10
//HardwareSerial serial(3); //rx_pin=16, tx_pin=17
BluetoothSerial bt;

typedef enum {READY, STREAM} state_t ;

state_t state = READY;
shoot_config shoot;

// the setup routine runs once when you press reset:
void setup() {
        ir.begin(9600);
        bt.begin("ESP32");
        pinMode(PIN_TRIGGER, INPUT);  
        ir.write(set_baudrate_9600,sizeof(set_baudrate_9600));
}


void loop() {
  unsigned long time_in_ms = millis();
  
      // if threads not exits
//       create thread check_trigger(time_in_ms, check_interval_trigger_in_ms, shoot* s);
//       create thread stream_ir_to_bt(time_in_ms, check_interval_ir_in_ms);

  switch(state) {
    case READY:
    //       create thread read_bt(time_in_ms, check_interval_bt_in_ms);

      break;
    case STREAM:
    //       create thread stream_to_ir(time_in_ms, check_interval_ir_in_ms);

      break;
  }



  
  //ir.write(my_serial_bytes,sizeof(my_serial_bytes));
}


void check_trigger(unsigned long *time_in_ms, shoot_config *shoot) {
  
  unsigned long old_time_status = 0;
  
  boolean trigger_old_status = false,
          trigger_pressed = false;
          
  while(true) {
    if (time_in_ms - old_time_status > CHECK_INTERVAL_TRIGGER_IN_MS) {
      //trigger_pressed = trigger_status();

      if (trigger_pressed =! trigger_old_status) {
        //mutex lock
        //send bt trigger pressed
        //mutex unlock
        //trigger old status = trigger pressed
      }
    }

    //ir_shoot(&shoot, &trigger pressed);
}

void ir_shoot(unsigned long *time_in_ms, boolean *trigger_pressed, shoot_config *shoot) {

  unsigned long shoot_timestamp = 0;
  typedef enum {READY, DELAY, SHOOTING, COOLDOWN} shoot_phase;
  
  switch(shoot_phase) {
    case READY:
      if (trigger_pressed) {
        shoot_timestamp = time_in_ms;
        shoot_phase = DELAY;
      }
      break;
    case DELAY:
      if (time_in_ms - shoot_timestamp >= shoot->trigger_delay_in_ms) {
        shoot_timestamp = time_in_ms;
        shoot_phase = SHOOTING;
      }
      break;
    case SHOOTING:
      if (time_in_ms - shoot_timestamp >= shoot->duration_max_in_ms) {
        shoot_timestamp = time_in_ms;
        shoot_phase = COOLDOWN;
      }
      else //ir send
      break;
    case COOLDOWN:
      if (time_in_ms - shoot_timestamp >= shoot->cooldown_in_ms) {
        shoot_phase = READY;
      }
      break;
  }
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