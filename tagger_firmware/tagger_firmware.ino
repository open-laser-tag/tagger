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
shoot_config shootconf;

// the setup routine runs once when you press reset:
void setup() {
        ir.begin(9600);
        bt.begin("ESP32");
        pinMode(PIN_TRIGGER, INPUT);  
        ir.write(set_baudrate_9600,sizeof(set_baudrate_9600));
}


void loop() {
  unsigned long time_in_ms = millis();
  boolean shoot_trigger_pressed,
          reload_trigger_pressed;
          
  
      // if threads not exits
//       create thread check_trigger(&time_in_ms, &shootconf, &shoot_trigger_pressed);
// create thread shoot(&shootconf, &shoot_trigger_pressed)
//create thread reload(&shootconf, &reload_trigger_pressed)
//       create thread stream_ir_to_bt();
//      create thread check_reload();

  switch(state) {
    case READY:
    //       create thread read_bt(&time_in_ms,);

      break;
    case STREAM:
    //       create thread stream_bt_to_ir(&state);

      break;
  }
}

//TODO use same function for reload trigger
void check_trigger(unsigned long *time_in_ms, shoot_config *shootconf) {
  
  unsigned long old_time = 0;
  
  boolean trigger_old_status = false,
          
  while(true) {
    if (time_in_ms - old_time > CHECK_INTERVAL_TRIGGER_IN_MS) {
      //trigger_pressed = trigger_status();

      //TODO?: send trigger status when status changes or every time interval?
      if (trigger_pressed =! trigger_old) {
        //mutex lock
        //send bt trigger pressed
        //mutex unlock
        //trigger old status = trigger pressed
      }
    }

    //ir_shoot(&shootconf, &trigger pressed);
}

//TODO: duration_min when trigger released, shoot mode, burst shot, magazine size
void ir_shoot(unsigned long *time_in_ms, boolean *trigger_pressed, shoot_config *shootconf) {

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
      if (time_in_ms - shoot_timestamp >= shootconf->trigger_delay_in_ms) {
        shoot_timestamp = time_in_ms;
        shoot_phase = SHOOTING;
      }
      break;
    case SHOOTING:
      if (time_in_ms - shoot_timestamp >= shootconf->duration_max_in_ms) {
        shoot_timestamp = time_in_ms;
        shoot_phase = COOLDOWN;
      }
      else //ir send shootconf->shoot bytes
      break;
    case COOLDOWN:
      if (time_in_ms - shoot_timestamp >= shootconf->cooldown_in_ms) {
        shoot_phase = READY;
      }
      break;
  }
}

void stream_ir_to_bt() {          
  while(true) {
    //TODO?: use time interval?
    while (ir.available() > 0) {
      ir_data = ir.read();
      //mutex lock
      //send bt it_data
      //mutex unlock
    }
  }
}

void stream_bt_to_ir(state_t *state) {
  while (true) {
    while (bt.available() > 0) {
      bt_data=bt.read();
      if (bt_data='\n') {
        state = READY,
        return; //TODO? end thread?
      }
      else ir.write(bt_data,sizeof(bt_data)); //TODO bt_data data type conversion?
    }
  }
}
