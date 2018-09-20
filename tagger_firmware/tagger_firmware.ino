#include "globals.h"
#include "BluetoothSerial.h"
#include <pthread.h>


void setup() {
  uint8_t set_baudrate_9600[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};

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
//       create thread check_trigger_interval(&time_in_ms, &shootconf, &shoot_trigger_pressed, CHECK_INTERVAL_TRIGGER_IN_MS);
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

void check_button_interval(unsigned long *time_in_ms, boolean *button_status, unsigned long time_interval, int button_pin) {
  
  unsigned long old_time = 0;
  boolean button_old_status = false;
          
  while(true) {
    if (time_in_ms - old_time > time_interval) {
      //button_status = read(button_pin)

      if (trigger_pressed =! trigger_old) {
        //mutex lock
        //send bt button status
        //mutex unlock
        //button_old_status = trigger pressed
      }
    }
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
  int ir_data = 0;   // for incoming serial data from ir
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
  int bt_data = 0; // the data given from smart phone app   
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
