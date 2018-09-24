#include "globals.h"
#include "BluetoothSerial.h"
#include <pthread.h>

HardwareSerial ir(1); // rx_pin=3, tx_pin=1
//HardwareSerial audio(2); //rx_pin=9, tx_handleInterruptpin=10
//HardwareSerial serial(3); //rx_pin=16, tx_pin=17
BluetoothSerial bt;

void setup() {
  uint8_t set_ir_baudrate_9600[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};
  
  ir.begin(9600);
  bt.begin("ESP32");
  pinMode(PIN_TRIGGER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TRIGGER), handle_trigger, CHANGE); 
  ir.write(set_ir_baudrate_9600,sizeof(set_ir_baudrate_9600));
}


void loop() {
  unsigned long time_in_ms = millis(); 
  
      // if threads not exits
//       create thread check_trigger_interval(&time_in_ms, &shootconf, &shoot_trigger_pressed, CHECK_INTERVAL_TRIGGER_IN_MS);
// create thread shoot(&shootconf, &shoot_trigger_pressed)
//create thread reload(&shootconf, &reload_trigger_pressed)
//       create thread stream_ir_to_bt();
//      create thread check_reload();

  switch(state) {
    case READY:
    //       create thread handle_bt();

      break;
    case STREAM:
    //       create thread stream_bt_to_ir();

      break;
  }
}

void handle_trigger() {
  trigger_status = !trigger_status;
  return;
}

//TODO: duration_min when trigger released, shoot mode, burst shot, magazine size
void ir_shoot(unsigned long *time_in_ms) {

  unsigned long shoot_timestamp = 0;
  typedef enum {READY, DELAY, SHOOTING, COOLDOWN} shoot_status;
  shoot_status shoot_phase;
  
  switch(shoot_phase) {
    case READY:
      if (trigger_status) {
        shoot_timestamp = *time_in_ms;
        shoot_phase = DELAY;
      }
      break;
    case DELAY:
      if (*time_in_ms - shoot_timestamp >= shootconf.trigger_delay_in_ms) {
        shoot_timestamp = *time_in_ms;
        shoot_phase = SHOOTING;
      }
      break;
    case SHOOTING:
      if (*time_in_ms - shoot_timestamp >= shootconf.duration_max_in_ms) {
        shoot_timestamp = *time_in_ms;
        shoot_phase = COOLDOWN;
      }
      else //ir send shootconf->shoot bytes
      break;
    case COOLDOWN:
      if (*time_in_ms - shoot_timestamp >= shootconf.cooldown_in_ms) {
        shoot_phase = READY;
      }
      break;
  }
}

void stream_to_bt() {       
  int ir_data = 0;   // for incoming serial data from ir
  bool trigger_old_status = false;

  while(true) {
    //TODO?: use time interval?
    if (trigger_status =! trigger_old_status) {
        //send bt trigger status
        trigger_old_status = trigger_status;
      }
    else {
      while (ir.available() > 0) {
        ir_data = ir.read();
        //send bt it_data
      }
    }
  }
}

void stream_bt_to_ir() {
  byte bt_data = 0; // the data given from smart phone app  
   
  while (true) {
    while (bt.available() > 0) {
      bt_data=bt.read();
      if (bt_data='\n') {
        state = READY;
        return; //TODO? end thread?
      }
      //else ir.write(bt_data,sizeof(bt_data)); //TODO bt_data data type conversion?
    }
  }
}

void write_to_config(byte bt_data[BT_MESSAGE_LENGHT], unsigned long *conf) {

  *conf = 0;
  
  for (int i=0; i<DATA_BYTES_LENGHT-1; i++) {
      shootconf.send_bytes <<= sizeof(byte);
      *conf |= (unsigned long)(bt_data[DATA_BYTES_POS+i]);
  }
}

void handle_bt() {
  byte bt_data[BT_MESSAGE_LENGHT];
  uint8_t byte_counter=0; 
  
  while(true) {
    while (bt.available() > 0) {
      bt_data[byte_counter]=bt.read();
      byte_counter++;

      //when command byte == COMMAND_STREAM go directly to stream mode
      if(bt_data[0] == COMMAND_STREAM) {
        state = STREAM;
        return; //TODO? end thread?
      }
      
      //receive BT_MESSAGE_LENGHT bytes then handle message
      else if(byte_counter >= BT_MESSAGE_LENGHT) {
        //check command byte
        if(bt_data[0] == COMMAND_WRITE) {

          //check address byte and write to conf
          if(bt_data[1] == ADDR_SEND_BYTES) write_to_config(bt_data, &shootconf.send_bytes);
          else if(bt_data[1] == ADDR_SHOOT_COOLDOWN) write_to_config(bt_data, &shootconf.cooldown_in_ms);
          else if(bt_data[1] == ADDR_SHOOT_DELAY) write_to_config(bt_data, &shootconf.trigger_delay_in_ms);
          else if(bt_data[1] == ADDR_SHOOT_DURATION_MIN) write_to_config(bt_data, &shootconf.duration_min_in_ms);
          else if(bt_data[1] == ADDR_SHOOT_DURATION_MAX) write_to_config(bt_data, &shootconf.duration_max_in_ms);
          else if(bt_data[1] == ADDR_SHOOT_MODE) write_to_config(bt_data, &shootconf.smode);
          else if(bt_data[1] == ADDR_BURST_AMOUNT_MIN) write_to_config(bt_data, &shootconf.burst_amount_min);
          else if(bt_data[1] == ADDR_BURST_AMOUNT_MAX) write_to_config(bt_data, &shootconf.burst_amount_max);
          else if(bt_data[1] == ADDR_BURST_COOLDOWN) write_to_config(bt_data, &shootconf.burst_cooldown_in_ms);
          else if(bt_data[1] == ADDR_MAGAZINE_SIZE) write_to_config(bt_data, &shootconf.magazine_size);        

          //TODO else error when unknown address
        }

        else if (bt_data[0] == COMMAND_READ) {
          //TODO send config via bt

        //error when unknown command byte
        // TODO else error
        memset(bt_data,0,BT_MESSAGE_LENGHT*sizeof(byte));
        byte_counter = 0;
        }
      }
    }
  }
}
