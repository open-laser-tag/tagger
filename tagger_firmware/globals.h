#ifndef globals_h
#define globals_h

#include <BLEDevice.h>
#include <BLE2902.h>
#include "misc.h"
#include "Ir_YS_IRTM.h"
#include "IRremote.h"

#define IR_RX_PIN 
#define ONBOARDLED_PIN 2
#define PIN_TRIGGER 21
#define DEBOUNCING_TIME_IN_MS 100
#define BAUD_RATE_IR 57600 //allowed values: 4800, 9600, 19200, 57600
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

unsigned long   latenz_timestamp=0,
                latenz=0;

TaskHandle_t        xHandle_handle_ir,
                    xHandle_refresh_trigger_status,
                    xHandle_send_latency;
Ir_YS_IRTM          ir(HARDWARE_SERIAL2, BAUD_RATE_IR);
HardwareSerial      usb(HARDWARE_SERIAL0);
BLECharacteristic   *trigger_char,
                    *ir_receive_char,
                    *ir_send_char,
                    *latency_char;
Led                 led(ONBOARDLED_PIN);
Button              trigger(PIN_TRIGGER);
IRrecv              irrecv(IR_PIN);
#endif
