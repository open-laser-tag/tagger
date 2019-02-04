/**
 * @file ir_and_trigger.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief infrared stuff and trigger handler
 * @date 2019-01-23
 *  
 */

#include "ir_and_trigger.h"

/**
 * @brief handle incoming infrared
 * This funciton reads the incoming infrared data and sends it via bluetooth. 
 * @param parameter just a dummy parameter
 */
void handle_ir(void * parameter) {

  decode_results results;

  while(true) {

    uint32_t ir_data=0;

    //ir modul YS-IRTM
    while(ir.available() > 0) {
      ir_data = ir.read();
      ir_receive_char->setValue(ir_data);
      usb.print("Incoming IR: ");
      usb.println(ir_data);
      ble_notify(ir_receive_char);
    }

    //ir receiver TSOP
    if (irrecv.decode(&results)) {
      usb.print("Incoming IR: ");
      usb.println(results.value, HEX);
      ir_receive_char->setValue((uint32_t&)results.value);
      ble_notify(ir_receive_char);
      irrecv.resume(); // Receive the next value
    }


    vTaskDelay(500 / portTICK_PERIOD_MS); //Block for 500ms.
  }
  return;
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
    ble_notify(trigger_char);

    portENTER_CRITICAL_ISR(&mux);
    count_trigger_interrupts = 0;
    portEXIT_CRITICAL_ISR(&mux);
  }
}
