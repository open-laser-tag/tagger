void handle_trigger() {
  vTaskResume(xHandle_refresh_trigger_status);
  return;
}

void refresh_trigger_status(void * parameter) {

  unsigned long last_time_refreshed = 0;
  
  while(true) {

    if (millis() - last_time_refreshed > DEBOUNCING_TIME_IN_MS) {
      trigger_pressed = !digitalRead(PIN_TRIGGER); // TODO read input
      usb.print("trigger pressed, status: ");
      usb.println(trigger_pressed);
      latenz_timestamp = millis();
      usb.print("device status: ");
      usb.println(deviceConnected);
      if(deviceConnected) vTaskResume(xHandle_send_bt);
      last_time_refreshed = millis();
    }

    if (shoot_phase == READY && trigger_pressed && state == AUTONOMOUS) {
      usb.print("start shooting ");
      usb.println(millis());
      shoot_timestamp = millis();
      shoot_phase = DELAY;
      vTaskResume (xHandle_send_ir);
    }
    vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()
  }
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
            usb.print("shoot phase ready (going to sleep) ");
            usb.println(millis());
            vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()
          } break;
            
          //do nothing until delay is over
          case DELAY: {
            unsigned long delayed_time = millis() - shoot_timestamp;
            if (delayed_time >= shootconf.trigger_delay_in_ms) {
              usb.print("shoot phase shooting ");
              usb.println(millis());
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
                usb.print("shoot phase cooldown (long shot) ");
                usb.println(millis());
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
                usb.print("shoot phase cooldown (short shot) ");
                usb.println(millis());
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
              usb.print("shoot phase ready ");
              usb.println(millis());
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
