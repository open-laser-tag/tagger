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
  vTaskResume(xHandle_refresh_trigger_status);
  return;
}

void refresh_trigger_status(void * parameter) {

  unsigned long last_time_refreshed = 0;
  
  while(true) {
    vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()


    if (millis() - last_time_refreshed > DEBOUNCING_TIME_IN_MS) {
      trigger.read_pin();
      usb.print("trigger status: ");
      usb.println(trigger.pressed);
      latenz_timestamp = millis();

      usb.println("sending via bt");
      trigger_char->setValue((int&)trigger.pressed);
      trigger_char->notify();
      last_time_refreshed = millis();
    }
  }
}
