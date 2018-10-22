#include "globals.h"
#include "send_ir.h"
#include "handle_ir.h"
void setup() {

  uint8_t set_ir_baudrate_9600[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};
  usb.begin(115200);   
  ir.begin(9600); 
  pinMode(ONBOARDLED_PIN, OUTPUT);

  ir.write(set_ir_baudrate_9600,sizeof(set_ir_baudrate_9600));


  xTaskCreate(
    handle_ir,            /* Task function. */
    "handle_ir",          /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    NULL                  /* Task handle to keep track of created task */
  );

  xTaskCreate(
    send_ir,              /* Task function. */
    "send_ir",            /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    NULL                  /* Task handle to keep track of created task */
  );

  xTaskCreate(
    blink_led,            /* Task function. */
    "blink_led",          /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    NULL                  /* Task handle to keep track of created task */
  );
}



void loop() {
  time_in_ms=millis();
  vTaskDelay(TIMING_RESOLUTION_IN_MS / portTICK_PERIOD_MS); //Block for TIMING_RESOLUTION_IN_MS
}



bool  led_status = true;
void blink_led(void * parameter) {
  while(true) {
    led_status = !led_status;
    digitalWrite(ONBOARDLED_PIN, led_status);
    vTaskDelay(1000 / portTICK_PERIOD_MS); //Block for 1000ms.
  }
}
