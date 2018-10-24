void handle_trigger() {
  
  trigger_pressed = !trigger_pressed;
  usb.println("trigger pressed");

  char value = 'A';
  //pCharacteristic->setValue(&value);
  //pCharacteristic->notify();
  usb.println("sent via bt: "+value);
  latenz_timestamp=millis();
  
  
  if (shoot_phase == READY && trigger_pressed) {

    if (state == AUTONOMOUS) {
      shoot_timestamp = millis();
      shoot_phase = DELAY;
      vTaskResume (xHandle_send_ir);
    }
  }
  
  return;
}

//TODO: magazine size, stream mode
void do_shoot_logic(void * parameter) {
  uint8_t test_data[5]={0xA1, 0xF1, 0xFF, 0xFF, 0xFF};
  //char test_data=0xA1F104FB12;

  while(true) {
    switch(state) {
      case STREAM: {
        vTaskSuspend(NULL); //suspend task until reactivated by TODO
      } break;

      case AUTONOMOUS: {
        switch(shoot_phase) {
          //ready to receive trigger signal
          case READY: {
            vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()
          } break;
            
          //do nothing until delay is over
          case DELAY: {
            unsigned long delayed_time = millis() - shoot_timestamp;
            if (delayed_time >= shootconf.trigger_delay_in_ms) {
              shoot_timestamp = millis();
              shoot_phase = SHOOTING;
            }
            else vTaskDelay((shootconf.trigger_delay_in_ms - delayed_time) / portTICK_PERIOD_MS); //delay task for rest of shoot delay time
          } break;
        
          //send ir, duration depending on trigger status
          case SHOOTING: {
            //trigger pressed: long shot / long burst
            if (trigger_pressed) {
              if (millis() - shoot_timestamp >= shootconf.duration_max_in_ms) {
                shoot_timestamp = millis();
                burst_counter++;
                shoot_phase = COOLDOWN;
    
    //            if (shootconf.burst_amount_max >= burst_counter) shoot_phase = BURST_COOLDOWN;
    //            else shoot_phase = COOLDOWN;
              }            
              //TODO disable interrupt to not disturb ir?
              else ir.write(test_data,sizeof(test_data));
            }
            
            //trigger released: short shot / short burst
            else { 
              if (millis() - shoot_timestamp >= shootconf.duration_min_in_ms) {
                shoot_timestamp = millis();
                burst_counter++;
                shoot_phase = COOLDOWN;
    
    //            if (shootconf.burst_amount_min >= burst_counter) shoot_phase = BURST_COOLDOWN;
    //            else shoot_phase = COOLDOWN;
              }
              //TODO disable interrupt to not disturb ir?
              else ir.write(test_data,sizeof(test_data));
            }
        
            
            //shoot_phase = READY;
          } break;
          
          //short cooldown, back to shooting
    //      case BURST_COOLDOWN:
    //        if (millis() - shoot_timestamp >= shootconf.cooldown_in_ms) {
    //          shoot_timestamp = millis();
    //          shoot_phase = SHOOTING;
    //        }
    //        
    //        break;
    
          //befor getting ready, wait for cooldown, depending on mode
          case COOLDOWN: {
            burst_counter = 0;
    
            unsigned long delayed_time = millis() - shoot_timestamp;
            if (delayed_time >= shootconf.cooldown_in_ms) {
              shoot_phase = READY;
              //leave cooldown in manual mode only when trigger released
              //if (shootconf.smode == SHOOT_MODE_MANUAL && !trigger_pressed) shoot_phase = READY;
              //else if (shootconf.smode == SHOOT_MODE_AUTO) shoot_phase = READY;
            }
            else vTaskDelay((shootconf.cooldown_in_ms - delayed_time) / portTICK_PERIOD_MS); //delay task for rest of cooldown
            
          } break;
        }
      } break;
    }
  }
}
