#include "globals.h"

unsigned long       latenz_timestamp=0,
                    latenz=0;
int                 count_trigger_interrupts=0;
uint32_t            last_bounce_time = 0;
portMUX_TYPE        mux = portMUX_INITIALIZER_UNLOCKED;
TaskHandle_t        xHandle_handle_ir,
                    xHandle_refresh_trigger_status,
                    xHandle_send_latency;
BLECharacteristic   *trigger_char,
                    *ir_receive_char,
                    *ir_send_char,
                    *latency_char,
                    *version_char;

Led                 led(ONBOARDLED_PIN);
Button              trigger(PIN_TRIGGER);
Ir_YS_IRTM          ir(HARDWARE_SERIAL2, BAUD_RATE_IR);
HardwareSerial      usb(HARDWARE_SERIAL0);