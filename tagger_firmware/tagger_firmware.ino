#include "globals.h"

/*    SETUP   */

void setup() {
  
  usb.begin(115200);   
  //trigger pin to interrupt
  attachInterrupt(digitalPinToInterrupt(PIN_TRIGGER), handle_trigger, CHANGE); //LOW, CHANGE, RISING, FALLINGd
  //init bluetooth low energy server and services
  init_ble();
  //init trigger and ir handling
  create_tasks();
  usb.println("blink led for finishing setup");
  //blink for telling that setup is done
  led.blinks();
}

/*    LOOP    */
//loop is not used, freeRTOS tasks are used instead
void loop() {  vTaskDelay(portMAX_DELAY); /*wait as much as posible ... */ }

void create_tasks() {
  xTaskCreate(
    handle_ir,            /* Task function. */
    "handle_ir",          /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    &xHandle_handle_ir    /* Task handle to keep track of created task */
  );

  
  xTaskCreate(
    refresh_trigger_status,              /* Task function. */
    "refresh_trigger_status",            /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    &xHandle_refresh_trigger_status      /* Task handle to keep track of created task */
  );

  xTaskCreate(
    send_latency,              /* Task function. */
    "send_latency",            /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    &xHandle_send_latency      /* Task handle to keep track of created task */
  );
}

void send_latency (void * parameter) {
  while(true) {
    //suspend until reactivated by onWrite of Ir_send_callbacks
    vTaskSuspend(NULL);
    //send latency via BT to app
    latency_char->setValue((int&)latenz);
    latency_char->notify();
    usb.println("latency sent via  bt");
  }
}
