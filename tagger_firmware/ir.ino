void handle_ir(void * parameter) {
  int             ir_data = 0;   // for incoming serial data from ir
  
  while(true) {
    while (ir.available() > 0) {
        ir_data = ir.read();
        usb.print("Incoming IR: ");
        usb.println(ir_data); //TODO: is this correct?
        ir_receive_char->setValue((int&)trigger_pressed);
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
    vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()
  }
}
