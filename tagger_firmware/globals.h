#include <BLEDevice.h>
#include <BLE2902.h>
#include "misc.h"

#define ONBOARDLED_PIN 2
#define PIN_TRIGGER 21
//#define PIN_RELOAD
#define DEBOUNCING_TIME_IN_MS 100
#define BAUD_RATE_IR 57600
#define BAUD_RATE_IR_4800_CODE 01
#define BAUD_RATE_IR_9600_CODE 02
#define BAUD_RATE_IR_19200_CODE 03
#define BAUD_RATE_IR_57600_CODE 04
#define HARDWARE_SERIAL0 0 // rx_pin=3, tx_pin=1 (usb)
#define HARDWARE_SERIAL1 1 // don't use this one
#define HARDWARE_SERIAL2 2 //rx_pin=16, tx_pin=17
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID                    "08dbb28a-ce2c-467a-9f12-4f15d574a220"
#define CHARACTERISTIC_TRIGGER_UUID     "756ad6a4-2007-4dc4-9173-72dc7d6b2627"
#define CHARACTERISTIC_IR_RECEIVE_UUID  "a95980fb-4f18-4b2e-a258-81bf77575117"
#define CHARACTERISTIC_IR_SEND_UUID     "8b91a0d2-5f7f-49cb-8939-4455d3d24b81"

unsigned long shoot_timestamp = 0,
              burst_counter=0,
              latenz_timestamp=0;

volatile bool   trigger_pressed=false;

TaskHandle_t  xHandle_handle_ir,
              xHandle_blink_led,
              xHandle_send_ir,
              xHandle_handle_bt,
              xHandle_send_bt,
              xHandle_refresh_trigger_status;

              
//BluetoothSerial bt;
HardwareSerial ir(2); //rx_pin=16, tx_pin=17
HardwareSerial usb(0); // rx_pin=3, tx_pin=1

bool deviceConnected = false;
BLECharacteristic   *trigger_char,
                    *ir_receive_char,
                    *ir_send_char;

Led                 led(ONBOARDLED_PIN);
