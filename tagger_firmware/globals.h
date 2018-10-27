#include <BLEDevice.h>
#include <BLE2902.h>

#define ONBOARDLED_PIN 2
#define DATA_BYTES_LENGHT 4 //sizeof unsinged long
#define COMMAND_WRITE 0x01
#define COMMAND_STREAM 0x02
#define COMMAND_READ 0x03 //TODO not used yet
#define ADDR_SEND_BYTES  0x01
#define ADDR_SHOOT_COOLDOWN 0x02
#define ADDR_SHOOT_DELAY 0x03
#define ADDR_SHOOT_DURATION_MIN 0x04
#define ADDR_SHOOT_DURATION_MAX 0x05
#define ADDR_SHOOT_MODE 0x06
#define ADDR_BURST_AMOUNT_MIN 0x07
#define ADDR_BURST_AMOUNT_MAX 0x08
#define ADDR_BURST_COOLDOWN 0x09
#define ADDR_MAGAZINE_SIZE 0x0A
#define PIN_TRIGGER 21
//#define PIN_RELOAD
#define SEND_trigger_pressed_INTERVAL 100   //use this or when status change? not used
#define CHECK_INTERVAL_BT_IN_MS 100 //not used
#define CHECK_INTERVAL_IR_IN_MS 100 //not used
#define SHOOT_MODE_MANUAL 0x00000001
#define SHOOT_MODE_AUTO 0x00000002
#define DEBOUNCING_TIME_IN_MS 100
#define BAUD_RATE_IR 57600
#define BAUD_RATE_IR_4800_CODE 01
#define BAUD_RATE_IR_9600_CODE 02
#define BAUD_RATE_IR_19200_CODE 03
#define BAUD_RATE_IR_57600_CODE 04
//#define TIMING_RESOLUTION_IN_MS 10

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID        "08dbb28a-ce2c-467a-9f12-4f15d574a220"
#define CHARACTERISTIC_UUID "756ad6a4-2007-4dc4-9173-72dc7d6b2627"

//struct for configuration of ir behaviour when in autonomous mode
typedef struct Shoot_config {
  //unsigned long = 4 bytes
  unsigned long send_bytes = 0x00FFFFFF, //standard value without bt connection
                cooldown_in_ms = 3000, //min 1000 ms break after end of shot
                trigger_delay_in_ms = 1000,  //0 ms delay after triggershoot
                duration_min_in_ms = 1000, //shot duration 100 ms
                duration_max_in_ms = 6000,
                smode = SHOOT_MODE_MANUAL,
                burst_amount_min = 1, //one shot per trigger
                burst_amount_max = 1,
                burst_cooldown_in_ms = 0,  //no burst shot
                magazine_size = 0, //infinte
                current_magazine_level = 0;
} Shoot_config;
Shoot_config    shootconf;

unsigned long shoot_timestamp = 0,
              burst_counter=0,
              latenz_timestamp=0;

volatile bool   trigger_pressed=false;

typedef enum {READY, DELAY, SHOOTING , COOLDOWN/*, BURST_COOLDOWN*/} shoot_status;
shoot_status shoot_phase = READY;

typedef enum    {AUTONOMOUS, STREAM}    state_t;
state_t         state = AUTONOMOUS;

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
BLECharacteristic *pCharacteristic;
