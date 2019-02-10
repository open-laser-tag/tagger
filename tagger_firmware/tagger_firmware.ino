/**
 * @file tagger_firmware.ino
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief main arduino sketch file
 * @date 2019-01-23
 * 
 * 
 */

#include "tagger.h"

/**
 * @brief arduino setup
 * This is the arduino setup funciton. It is run once at startup.
 * Inits are done here and RTOS tasks are started here.
 * 
 */
void setup() {
  
  usblog.begin(115200);
  usblog.println();
  usblog.println("*******************************************");
  usblog.println("Hello, this is OpenLT Tagger.");
  usblog.print("Tagger Firmware version: ");
  usblog.println(GIT_TAG);
  usblog.println("*******************************************");
  usblog.println();


  usblog.println("DEBUG: Starting init...");
  //create semaphores
  init_mutex();
  //trigger pin to interrupt
  attachInterrupt(digitalPinToInterrupt(PIN_TRIGGER), handle_trigger, CHANGE); //LOW, CHANGE, RISING, FALLING
  //init bluetooth low energy server, services, characteristics
  init_ble();
  //init trigger and ir handling
  create_tasks();
  //blink for telling that setup is done
  usblog.debugln("Enabling IRin...");
  irrecv.enableIRIn(); // Start the receiver
  usblog.debugln("Enabled IRin");

  usblog.debugln("Init finished: blink LED");
  led.blinks();
}

/**
 * @brief arduino loop
 * This is the arduino loop function. It is not used, instead RTOS tasks are used.
 * That's why the loop task is delayed max.
 * 
 */
void loop() {  vTaskDelay(portMAX_DELAY); /*wait as much as posible ... */ }

/**
 * @brief Create all tasks object
 * This function creates all RTOS tasks. 
 */
void create_tasks() {
  xTaskCreate(
    handle_ir,            /* Task function. */
    "handle_ir",          /* name of task. */
    10000,                /* Stack size of task */
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

  usblog.debugln("send latency task started");

  while(true) {
    //suspend until reactivated by onWrite of Ir_send_callbacks
    vTaskSuspend(NULL);
    //send latency via BT to app
    latency_char->setValue(latenz);
    ble_notify(latency_char);
    usblog.debugln("latency sent via bt");
  }
}

/**
 * @brief create semaphores
 * Create mutex type semaphore for USB and BT communication.
 * Call this before using ble_notify or debug logger outputs.
 */
void init_mutex() {
  usblog.println("DEBUG: creating USB mutex...");
  xMutex_USB = xSemaphoreCreateMutex();
  if( xMutex_USB != NULL ) usblog.debugln("USB Mutex successfully created.");
  else usblog.println("Error: Could not create USB Mutex");

  usblog.debugln("creating BT mutex...");
  xMutex_BT = xSemaphoreCreateMutex();
  if( xMutex_BT != NULL ) usblog.debugln("BT Mutex successfully created.");
  else usblog.errorln("Could not create BT Mutex");


}
