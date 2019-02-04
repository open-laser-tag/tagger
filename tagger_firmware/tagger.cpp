/**
 * @file tagger.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief global variables
 * @date 2019-01-23
 *  
 */

#include "tagger.h"

uint32_t            latenz_timestamp=0,
                    latenz=0,
                    last_bounce_time = 0;

uint16_t            count_trigger_interrupts=0;

portMUX_TYPE        mux = portMUX_INITIALIZER_UNLOCKED;

TaskHandle_t        xHandle_handle_ir,
                    xHandle_refresh_trigger_status,
                    xHandle_send_latency;

/**
 * @brief bluetooth mutex
 * This mutex is used to lock/unlock the bluetooth send communication.
 */
SemaphoreHandle_t    xMutex_BT;

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

Led                 led(ONBOARDLED_PIN);
Button              trigger(PIN_TRIGGER);
Ir_YS_IRTM          ir(HARDWARE_SERIAL2, BAUD_RATE_IR);
HardwareSerial      usb(HARDWARE_SERIAL0);
IRrecv              irrecv(IR_RECV_PIN);