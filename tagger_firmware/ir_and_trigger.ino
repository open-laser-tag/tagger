void handle_ir(void * parameter) {
  int             ir_data = 0;   // for incoming serial data from ir
  
  while(true) {
    while (ir.available() > 0) {
        ir_data = ir.read();
        usb.print("Incoming IR: ");
        usb.println(ir_data); //TODO: is this correct?
        ir_receive_char->setValue((int&)ir_data);
        ir_receive_char->notify();
    }
    vTaskDelay(500 / portTICK_PERIOD_MS); //Block for 500ms.
  }
}

void handle_trigger() {
  portENTER_CRITICAL_ISR(&mux);
  count_trigger_interrupts++;
  trigger.read_pin();
  debounce_timeout = xTaskGetTickCount();
  portEXIT_CRITICAL_ISR(&mux);
  
  vTaskResume(xHandle_refresh_trigger_status);

  return;
}

/*
 * tried new debounce, source: https://www.switchdoc.com/2018/04/esp32-tutorial-debouncing-a-button-press-using-interrupts/
 */

void refresh_trigger_status(void * parameter) {

  unsigned long last_time_refreshed = 0;
  uint32_t debounce_timeout_save;
  bool curr_trigger_state,
       trigger_state;
  int count_trigger_interrupts_save;
  //bool testdata=false;

  
  while(true) {
    vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()

    portENTER_CRITICAL_ISR(&mux);
    count_trigger_interrupts_save  = count_trigger_interrupts;
    debounce_timeout_save = debounce_timeout;
    trigger_state  = trigger.pressed;
    portEXIT_CRITICAL_ISR(&mux);

    trigger.read_pin();
    curr_trigger_state = trigger.pressed;

    if ((debounce_timeout_save != 0) //interrupt has triggered
        && (curr_trigger_state == trigger.pressed) // pin is still in the same state as when intr triggered
        && (millis() - debounce_timeout_save > DEBOUNCETIME )) {
    //if (millis() - last_time_refreshed > DEBOUNCETIME) {
      //wait for 10 ms for debouncing
      //vTaskDelay(100);
      
      //trigger.read_pin();
      usb.print("Button Interrupt Triggered times");
      usb.println(count_trigger_interrupts_save);
      usb.print("time since last trigger");
      usb.println(millis() - debounce_timeout_save);
      usb.print("trigger status: ");
      usb.println(trigger.pressed);
      latenz_timestamp = millis();
  
      usb.println("sending trigger status via bt");
      //testdata = !testdata;
      trigger_char->setValue((int&)trigger.pressed);
      trigger_char->notify();
      last_time_refreshed = millis();

     portENTER_CRITICAL_ISR(&mux); // can't change it unless, atomic - Critical section
      count_trigger_interrupts = 0; // acknowledge keypress and reset interrupt counter
      portEXIT_CRITICAL_ISR(&mux);
    }
  }
}
