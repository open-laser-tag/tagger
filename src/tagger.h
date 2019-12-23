/**
 * @file globals.h
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief global variables
 * @date 2019-01-23
 * 
 */
#ifndef tagger_h
#define tagger_h

/* ++++++ */
/* config */
// these configs can safely be editted for each tagger seperatly

/* RGB LEDs */
//choose one LED type
//#define LED_TYPE_APA102
#define LED_TYPE_NEOPIXEL
#define LED_CLOCK_PIN 18 //only used for spi leds like APA102
#define LED_DATA_PIN 19
#define NUM_LEDS 4
#define LED_OVERALL_BRIGHTNESS 20 //scale 0 to 255
//FASTLED_ESP32_I2S is defined to tell FastLED to use I2S driver instead of RMT. Reference for this option in FastLED/platforms/esp/32/clockless_i2s_esp32.h. There is a conflict when using RMT with both LED and IR, although it should be possible according to the options in FastLED/platforms/esp/32/clockless_rmt_esp32.h. Maybe there is a connection to this issue: https://github.com/FastLED/FastLED/issues/835
#define FASTLED_ESP32_I2S true
#define LED_INDEX_BT 0
#define LED_INDEX_PLAYER_STATUS 1
#define LED_INDEX_TEAM 2
#define LED_INDEX_SHOOT 3
#define COLOR_BT_CONNECTION_ON 0x0000FF
#define COLOR_BT_CONNECTION_OFF 0xFF0000
#define COLOR_PLAYER_STATUS_ON 0x00FF00
#define COLOR_PLAYER_STATUS_OFF 0xFF0000
#define ONBOARDLED_PIN 2

/* Infra Red */
#define IR_RECV_FRONT_PIN 27
#define IR_RECV_RIGHT_PIN 26
#define IR_RECV_LEFT_PIN 25
#define IR_RMT_TX_CHANNEL RMT_CHANNEL_0 /*!< RMT channel for transmitter */
#define IR_RMT_RX_FRONT_CHANNEL RMT_CHANNEL_1
#define IR_RMT_TX_GPIO_NUM GPIO_NUM_22 /*!< GPIO number for transmitter signal */

/* Trigger Button */
#define PIN_TRIGGER 21
#define DEBOUNCETIME 10

/* OTA */
#define OTA_WIFI_SSID "wlanssid"
#define OTA_WIWI_PASSWORD "wlanpassword"
#define EEPROM_ADDR_OTA 0
#define EEPROM_SIZE 1

/* Timings */
#define PLAYER_DOWNTIME_IN_MS 3000
#define TEAM_SELECTION_TIME_IN_MS 5000

/* ++++++++ */
/* includes */
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLE2902.h>
#include "misc.h"
#include "ir_and_trigger.h"
#include "ble.h"
#include <FastLED.h>
#include <esp_log.h>
#include "ota.h"
#include <EEPROM.h>
#include "IR32/src/IRSend.h"
#include "IR32/src/IRRecv.h"

/* git version */
#if __has_include("git_info.h")
#include "git_info.h"
#else
#define GIT_TAG "v0.1.3"
#endif

/* ++++++ */
/* macros */
// Don't edit this section. It has to be equal on all taggers.
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "08dbb28a-ce2c-467a-9f12-4f15d574a220"
#define CHARACTERISTIC_TRIGGER_UUID "756ad6a4-2007-4dc4-9173-72dc7d6b2627"
#define CHARACTERISTIC_IR_RECEIVE_UUID "a95980fb-4f18-4b2e-a258-81bf77575117"
#define CHARACTERISTIC_IR_SEND_UUID "8b91a0d2-5f7f-49cb-8939-4455d3d24b81"
#define CHARACTERISTIC_LATENCY_UUID "60e44cef-5a43-407b-8d1a-bce02377dcfd"
#define CHARACTERISTIC_VERSION_UUID "563c139f-3eda-4c88-9fc3-be987038fa6a"
#define CHARACTERISTIC_LED_UUID "7a4821c2-80f0-4eba-8070-d659d31e43de"
#define CHARACTERISTIC_OTA_UUID "eebc6352-2559-40f1-bda8-2715e7c07fbd"
#define IR_PROTOCOL "NEC"              //choose from timing groups lib/IR32/src/IR32.h

/* global variables*/
extern const uint32_t color_team[];
extern uint32_t last_time_button_pressed_timestamp,
    latency,
    last_bounce_time;
extern const uint32_t ir_msg[];
extern uint16_t count_trigger_interrupts,
    msg_nr;
extern uint8_t team;
extern bool team_selection;
extern portMUX_TYPE mux;
extern TaskHandle_t xHandle_handle_ir,
    xHandle_refresh_trigger_status,
    xHandle_send_latency,
    xHandle_handle_player_status;
extern SemaphoreHandle_t xMutex_BT;
extern BLECharacteristic *trigger_char,
    *ir_receive_char,
    *ir_send_char,
    *latency_char,
    *version_char,
    *led_char,
    *ota_char;
extern IRSend ir_led;
extern IRRecv ir_recv_front;
extern Led led;
extern Button trigger;
extern CRGB leds[NUM_LEDS];
extern bool player_is_on;

/* function declerations */
void setup();
void loop();
void create_tasks();
void send_latency(void *parameter);
void init_mutex();
#endif // tagger_h
