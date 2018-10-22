void handle_ir(void * parameter) {
  int             ir_data = 0;   // for incoming serial data from ir
  
  while(true) {
    while (ir.available() > 0) {
        ir_data = ir.read();
        usb.print("Incoming IR: ");
        usb.print(ir_data); //TODO: is this correct?
        usb.print("\n");
    }
    vTaskDelay(500 / portTICK_PERIOD_MS); //Block for 500ms.
  }
}
