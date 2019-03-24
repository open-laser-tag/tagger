/**
 * @file globals.h
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief global variables
 * @date 2019-01-23
 * 
 */

#ifndef tagger_h
#define tagger_h
//this is a hardware identifier for FastLED
#define ESP32
//I don't know why this is needed. It is for IRremote.h/IRremoteInt.h 
#define ARDUINO 101

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLE2902.h>
#include "misc.h"
#include "Ir_YS_IRTM.h"
#include "ir_and_trigger.h"
#include "ble.h"
#include <IRremote.h>
#include <FastLED.h>
#include "git_info.h" //if this file is missing, type make

//for FastLED.h
#define NUM_LEDS 1
#define DATA_PIN 13

#define IR_RECV_FRONT_PIN 27
#define IR_RECV_RIGHT_PIN 26
#define IR_RECV_LEFT_PIN 25
#define PIN_TRIGGER 21
#define ONBOARDLED_PIN 2
#define DEBOUNCETIME 10
#define BAUD_RATE_IR 9600 //allowed values: 4800, 9600, 19200, 57600
#define HARDWARE_SERIAL0 0 // rx_pin=3, tx_pin=1 (usb)
#define HARDWARE_SERIAL1 1 // don't use this one
#define HARDWARE_SERIAL2 2 //rx_pin=16, tx_pin=17
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID                    "08dbb28a-ce2c-467a-9f12-4f15d574a220"
#define CHARACTERISTIC_TRIGGER_UUID     "756ad6a4-2007-4dc4-9173-72dc7d6b2627"
#define CHARACTERISTIC_IR_RECEIVE_UUID  "a95980fb-4f18-4b2e-a258-81bf77575117"
#define CHARACTERISTIC_IR_SEND_UUID     "8b91a0d2-5f7f-49cb-8939-4455d3d24b81"
#define CHARACTERISTIC_LATENCY_UUID     "60e44cef-5a43-407b-8d1a-bce02377dcfd"
#define CHARACTERISTIC_VERSION_UUID     "563c139f-3eda-4c88-9fc3-be987038fa6a"

extern uint32_t             latenz_timestamp,
                            latenz,
                            last_bounce_time;

extern uint16_t             count_trigger_interrupts;

extern portMUX_TYPE         mux;

extern TaskHandle_t         xHandle_handle_ir,
                            xHandle_refresh_trigger_status,
                            xHandle_send_latency;

extern SemaphoreHandle_t    xMutex_BT,
                            xMutex_USB;

extern BLECharacteristic    *trigger_char,
                            *ir_receive_char,
                            *ir_send_char,
                            *latency_char,
                            *version_char;

extern Ir_YS_IRTM           ir; 
extern Led                  led;
extern Button               trigger;
extern IRrecv               irrecv_front,
                            irrecv_right,
                            irrecv_left;
extern Logger               usblog;

extern CRGB                 leds[NUM_LEDS];
#endif // tagger_h
