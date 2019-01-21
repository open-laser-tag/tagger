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

  last_bounce_time = xTaskGetTickCount();

  portENTER_CRITICAL_ISR(&mux);
  count_trigger_interrupts++;
  portEXIT_CRITICAL_ISR(&mux);
  
  vTaskResume(xHandle_refresh_trigger_status);

  return;
}

void refresh_trigger_status(void * parameter) {
  
  while(true) {
    vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()

    //wait until the last bounce is longer ago than DEBOUNCETIME
    while (xTaskGetTickCount() - last_bounce_time < DEBOUNCETIME ) vTaskDelay(10);

    //refresh trigger.pressed
    trigger.read_pin();

    usb.print("Button Interrupt Triggered times: ");
    usb.println(count_trigger_interrupts);
    usb.print("time since last trigger: ");
    usb.println(xTaskGetTickCount() - last_bounce_time);
    usb.print("trigger status: ");
    usb.println(trigger.pressed);
    latenz_timestamp = millis();

    usb.println("sending trigger status via bt");
    trigger_char->setValue((int&)trigger.pressed);
    trigger_char->notify();

    portENTER_CRITICAL_ISR(&mux);
    count_trigger_interrupts = 0;
    portEXIT_CRITICAL_ISR(&mux);
  }
}
