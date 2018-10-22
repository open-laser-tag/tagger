void handle_trigger() {
  
  trigger_pressed = !trigger_pressed;

  if (shoot_phase == READY && trigger_pressed) {
    shoot_timestamp = time_in_ms;
    shoot_phase = DELAY;
    vTaskResume (xHandle_send_ir);
  }
  
  return;
}

void send_ir(void * parameter) {
  uint8_t test_data[5]={0xA1, 0xF1, 0x04, 0xFB, 0x12};
  //char test_data=0xA1F104FB12;

  while(true) {
    switch(shoot_phase) {
      //ready to receive trigger signal
      case READY: {
        vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()
      }break;
        

        
      //do nothing until delay is over
      case DELAY: {
        unsigned long delayed_time = time_in_ms - shoot_timestamp;
        if (delayed_time >= shootconf.trigger_delay_in_ms) {
          shoot_timestamp = time_in_ms;
          shoot_phase = SHOOTING;
        }
        else vTaskDelay((shootconf.trigger_delay_in_ms - delayed_time) / portTICK_PERIOD_MS); //delay task for rest of shoot delay time
      }break;
    
      //send ir, duration depending on trigger status
      case SHOOTING: {
        ir.write(test_data,sizeof(test_data));
        shoot_phase = READY;
      }break;
    }
  }
}
//            //trigger pressed: long shot / long burst
//            if (trigger_pressed) { 
//              if (time_in_ms - shoot_timestamp >= shootconf.duration_max_in_ms) {
//                shoot_timestamp = time_in_ms;
//                shoot_phase = COOLDOWN;
//                burst_counter++;
//
//                if (shootconf.burst_amount_max >= burst_counter) shoot_phase = BURST_COOLDOWN;
//                else shoot_phase = COOLDOWN;
//              }            
//              //TODO disable interrupt to not disturb ir?
//              //else //TODO ir send shootconf->shoot bytes
//            }
//            
//            //trigger released: short shot / short burst
//            else { 
//              if (time_in_ms - shoot_timestamp >= shootconf.duration_min_in_ms) {
//                shoot_timestamp = time_in_ms;
//                shoot_phase = COOLDOWN;
//                burst_counter++;
//
//                if (shootconf.burst_amount_min >= burst_counter) shoot_phase = BURST_COOLDOWN;
//                else shoot_phase = COOLDOWN;
//              }
//              //TODO disable interrupt to not disturb ir?
//              //else //TODO ir send shootconf->shoot bytes
//            }
  
          //short cooldown, back to shooting
//          case BURST_COOLDOWN:
//            if (time_in_ms - shoot_timestamp >= shootconf.cooldown_in_ms) {
//              shoot_timestamp = time_in_ms;
//              shoot_phase = SHOOTING;
//            }
//            
//            break;
//  
//          //befor getting ready, wait for cooldown, depending on mode
//          case COOLDOWN:
//            burst_counter = 0;
//            
//            if (time_in_ms - shoot_timestamp >= shootconf.cooldown_in_ms) {
//              
//              //leave cooldown in manual mode only when trigger released
//              if (shootconf.smode == SHOOT_MODE_MANUAL && !trigger_pressed) shoot_phase = READY;
//              else if (shootconf.smode == SHOOT_MODE_AUTO) shoot_phase = READY;
//              }
//          break;
          


  
  //vTaskDelete( NULL );
//}



//TODO: magazine size
//void *send_ir(void *i) {
//
//  unsigned long shoot_timestamp = 0,
//                burst_counter=0;
//  typedef enum {READY, DELAY, SHOOTING, COOLDOWN, BURST_COOLDOWN} shoot_status;
//  shoot_status shoot_phase;
//
//  while(true) {
//    switch(state) {
//      case STREAM:
//        // do nothing
//        break;
//        
//      case AUTONOMOUS:
//  
//        switch(shoot_phase) {
//  
//          //ready to receive trigger signal
//          case READY:
//            if (trigger_pressed) {
//              shoot_timestamp = time_in_ms;
//              shoot_phase = DELAY;
//            }
//            break;
//  
//          //do nothing until delay is over
//          case DELAY:
//            if (time_in_ms - shoot_timestamp >= shootconf.trigger_delay_in_ms) {
//              shoot_timestamp = time_in_ms;
//              shoot_phase = SHOOTING;
//            }
//            break;
//  
//          //send ir, duration depending on trigger status
//          case SHOOTING:
//          
//            //trigger pressed: long shot / long burst
//            if (trigger_pressed) { 
//              if (time_in_ms - shoot_timestamp >= shootconf.duration_max_in_ms) {
//                shoot_timestamp = time_in_ms;
//                shoot_phase = COOLDOWN;
//                burst_counter++;
//
//                if (shootconf.burst_amount_max >= burst_counter) shoot_phase = BURST_COOLDOWN;
//                else shoot_phase = COOLDOWN;
//              }            
//              //TODO disable interrupt to not disturb ir?
//              //else //TODO ir send shootconf->shoot bytes
//            }
//            
//            //trigger released: short shot / short burst
//            else { 
//              if (time_in_ms - shoot_timestamp >= shootconf.duration_min_in_ms) {
//                shoot_timestamp = time_in_ms;
//                shoot_phase = COOLDOWN;
//                burst_counter++;
//
//                if (shootconf.burst_amount_min >= burst_counter) shoot_phase = BURST_COOLDOWN;
//                else shoot_phase = COOLDOWN;
//              }
//              //TODO disable interrupt to not disturb ir?
//              //else //TODO ir send shootconf->shoot bytes
//            }
//            break;
//  
//          //short cooldown, back to shooting
//          case BURST_COOLDOWN:
//            if (time_in_ms - shoot_timestamp >= shootconf.cooldown_in_ms) {
//              shoot_timestamp = time_in_ms;
//              shoot_phase = SHOOTING;
//            }
//            
//            break;
//  
//          //befor getting ready, wait for cooldown, depending on mode
//          case COOLDOWN:
//            burst_counter = 0;
//            
//            if (time_in_ms - shoot_timestamp >= shootconf.cooldown_in_ms) {
//              
//              //leave cooldown in manual mode only when trigger released
//              if (shootconf.smode == SHOOT_MODE_MANUAL && !trigger_pressed) shoot_phase = READY;
//              else if (shootconf.smode == SHOOT_MODE_AUTO) shoot_phase = READY;
//              }
//          break;
//          
//        }
//      break;
//    }
//  }
//}
