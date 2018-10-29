#include "globals.h"

/*    SETUP   */

void setup() {
  
  usb.begin(115200);   
  //pinMode(ONBOARDLED_PIN, OUTPUT);
  pinMode(PIN_TRIGGER, INPUT_PULLUP);
  //trigger pin to interrupt
  attachInterrupt(digitalPinToInterrupt(PIN_TRIGGER), handle_trigger, CHANGE); //LOW, CHANGE, RISING, FALLING
  //init ir with default baud rate
  ir.begin(9600); 
  //set custom baud rate
  uint8_t set_ir_baudrate[5]={0xA1, 0xF3, BAUD_RATE_IR_57600_CODE, 0x00, 0x00};
  ir.write(set_ir_baudrate,sizeof(set_ir_baudrate));
  ir.end();
  ir.begin(BAUD_RATE_IR); 

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
}
