#include "globals.h"
#include "handle_ir.h"
#include "handle_bt.h"
#include "send_ir.h"


void setup() {
  
  usb.begin(115200);   
  ir.begin(9600); 

  //if(bt.begin("ESP32")) usb.println("BT successfully");
  //else usb.println("An error occurred initializing Bluetooth");
  
  pinMode(ONBOARDLED_PIN, OUTPUT);
  pinMode(PIN_TRIGGER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TRIGGER), handle_trigger, FALLING); //LOW, CHANGE, RISING, FALLING

  uint8_t set_ir_baudrate_9600[5]={0xA1, 0xF3, 0x02, 0x00, 0x00};
  ir.write(set_ir_baudrate_9600,sizeof(set_ir_baudrate_9600));
  
  init_ble();

  xTaskCreate(
    handle_ir,            /* Task function. */
    "handle_ir",          /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    &xHandle_handle_ir    /* Task handle to keep track of created task */
  );

  xTaskCreate(
    blink_led,            /* Task function. */
    "blink_led",          /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    &xHandle_blink_led    /* Task handle to keep track of created task */
  );

  xTaskCreate(
    do_shoot_logic,              /* Task function. */
    "do_shoot_logic",            /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                 /* parameter of the task */
    1,                    /* priority of the task */
    &xHandle_send_ir      /* Task handle to keep track of created task */
  );
}

void loop() {  vTaskDelay(portMAX_DELAY); /*wait as much as posible ... */ }

bool  led_status = true;
void blink_led(void * parameter) {
  while(true) {
    led_status = !led_status;
    digitalWrite(ONBOARDLED_PIN, led_status);
    vTaskDelay(1000 / portTICK_PERIOD_MS); //Block for 1000ms.
  }
}
