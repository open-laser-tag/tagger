/**
 * @file tagger.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief global variables
 * @date 2019-01-23
 *  
 */

#include "tagger.h"

uint32_t            latency_timestamp=0,
                    latency=0,
                    last_bounce_time = 0;

uint16_t            count_trigger_interrupts=0,
                    msg_nr=0;

portMUX_TYPE        mux = portMUX_INITIALIZER_UNLOCKED;

TaskHandle_t        xHandle_handle_ir,
                    xHandle_refresh_trigger_status,
                    xHandle_send_latency;

/**
 * @brief bluetooth mutex
 * This mutex is used to lock/unlock the bluetooth communication.
 */
SemaphoreHandle_t    xMutex_BT;

/**
 * @brief USB mutex
 * This mutex is used to lock/unlock the USB communication.
 */
SemaphoreHandle_t    xMutex_USB;

/**
 * @brief send trigger status
 * Trigger characteristic is used to send trigger status.
 */
BLECharacteristic   *trigger_char;

/**
 * @brief send received ir data
 * This characteristic is used to send received ir data.
 */
BLECharacteristic   *ir_receive_char;

/**
 * @brief send data via ir
 * This characteristic is used to receive data which should be sent via infrared.
 */
BLECharacteristic   *ir_send_char;

/**
 * @brief send latency
 * This characteristic is used to send latency value.
 * Latency is calculated between recognized trigger change and shooting of ir.
 */
BLECharacteristic   *latency_char;

/**
 * @brief send firmware version
 * This characteristic is used to send the firmware version.
 * Version number is defined in git_info.h
 */
BLECharacteristic   *version_char;

BLECharacteristic   *led_char;

Led                     led(ONBOARDLED_PIN);
Button                  trigger(PIN_TRIGGER);
Esp32_infrared_nec_tx   ir_led;
IRrecv                  irrecv_front(IR_RECV_FRONT_PIN),
                        irrecv_right(IR_RECV_RIGHT_PIN),
                        irrecv_left(IR_RECV_LEFT_PIN);
Logger                  usblog(HARDWARE_SERIAL0, &xMutex_USB);
CRGB                    leds[NUM_LEDS];

/**
 * @brief arduino setup
 * This is the arduino setup function. It is run once at startup.
 * Inits are done here and RTOS tasks are started here.
 */
void setup() {
  esp_log_level_set("*", ESP_LOG_INFO);

  usblog.begin(115200);
  usblog.println();
  usblog.println("*******************************************");
  usblog.println("Hello, this is Open Laser Tag Tagger.");
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

  usblog.debugln("Enabling IRin...");
  irrecv_front.enableIRIn(); // Start the receiver
  //multiple receiver not working yet
  // irrecv_right.enableIRIn(); // Start the receiver
  // irrecv_left.enableIRIn(); // Start the receiver
  usblog.debugln("Enabled IRin");

  //init fast LED strip
  // causes the esp32 to crash in combination with IR LED tx rmt
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // leds[0].setRGB( 10, 10, 10);
  // FastLED.show();

  usblog.debugln("Init IR LED");
  ir_led.init(IR_RMT_TX_CHANNEL, IR_RMT_TX_GPIO_NUM);

  //init trigger and ir handling
  create_tasks();

  //blink for telling that setup is done
  usblog.debugln("Init finished: blink LED");
  led.blinks();
}

/**
 * @brief arduino loop
 * This is the arduino loop function. It is not used, instead RTOS tasks are used.
 * That's why the loop task is delayed max.
 * 
 */
void loop() {  vTaskDelay(portMAX_DELAY); /*wait as much as possible ... */ }

/**
 * @brief Create all tasks object
 * This function creates all RTOS tasks. 
 */
void create_tasks() {
  xTaskCreate(
    handle_ir,                        /* Task function. */
    "handle_ir",                      /* name of task. */
    2048,                            /* Stack size of task */
    NULL,                             /* parameter of the task */
    1,                                /* priority of the task */
    &xHandle_handle_ir                /* Task handle to keep track of created task */
  );

  xTaskCreate(
    refresh_trigger_status,           /* Task function. */
    "refresh_trigger_status",         /* name of task. */
    2048,                            /* Stack size of task */
    NULL,                             /* parameter of the task */
    1,                                /* priority of the task */
    &xHandle_refresh_trigger_status   /* Task handle to keep track of created task */
  );

  xTaskCreate(
    send_latency,                     /* Task function. */
    "send_latency",                   /* name of task. */
    2048,                            /* Stack size of task */
    NULL,                             /* parameter of the task */
    1,                                /* priority of the task */
    &xHandle_send_latency             /* Task handle to keep track of created task */
  );
}

void send_latency (void * parameter) {

  usblog.debugln("send latency task started");

  while(true) {
    //suspend until reactivated by onWrite of Ir_send_callbacks
    vTaskSuspend(NULL);
    //send latency via BT to app
    latency_char->setValue(latency);
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