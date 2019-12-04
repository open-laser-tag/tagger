/**
 * @file tagger.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief global variables
 * @date 2019-01-23
 *  
 */

#include "tagger.h"
const uint32_t color_team[] =
    {
        0x0000FF,
        0x00FF00,
        0x00FFFF,
        0xFF0000,
        0xFF00FF,
        0xFFFF00,
        0xFFFFFF,
};

uint32_t last_time_button_pressed_timestamp = 0,
         latency = 0,
         last_bounce_time = 0;

const uint16_t ir_msg[] =
    {
        0xFFFF,
        0xFFFE,
        0xFFFD,
        0xFFFC,
        0xFFFB,
        0xFFFA,
        0xFFF9,
};

uint16_t count_trigger_interrupts = 0,
         msg_nr = 0;

uint8_t team = 0;

extern bool team_selection = false;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

TaskHandle_t xHandle_handle_ir,
    xHandle_refresh_trigger_status,
    xHandle_send_latency,
    xHandle_handle_player_status;

/**
 * @brief bluetooth mutex
 * This mutex is used to lock/unlock the bluetooth communication.
 */
SemaphoreHandle_t xMutex_BT;

/**
 * @brief send trigger status
 * Trigger characteristic is used to send trigger status.
 */
BLECharacteristic *trigger_char;

/**
 * @brief send received ir data
 * This characteristic is used to send received ir data.
 */
BLECharacteristic *ir_receive_char;

/**
 * @brief send data via ir
 * This characteristic is used to receive data which should be sent via infrared.
 */
BLECharacteristic *ir_send_char;

/**
 * @brief send latency
 * This characteristic is used to send latency value.
 * Latency is calculated between recognized trigger change and shooting of ir.
 */
BLECharacteristic *latency_char;

/**
 * @brief send firmware version
 * This characteristic is used to send the firmware version.
 * Version number is defined in git_info.h
 */
BLECharacteristic *version_char;

BLECharacteristic *led_char;

Led led(ONBOARDLED_PIN);
Button trigger(PIN_TRIGGER);
Esp32_infrared_nec_tx ir_led;
IRrecv irrecv_front(IR_RECV_FRONT_PIN),
    irrecv_right(IR_RECV_RIGHT_PIN),
    irrecv_left(IR_RECV_LEFT_PIN);
CRGB leds[NUM_LEDS];
bool player_is_on = true;

/**
 * @brief arduino setup
 * This is the arduino setup function. It is run once at startup.
 * Inits are done here and RTOS tasks are started here.
 */
void setup()
{
    const char *logtag = "setup";

    Serial.begin(115200);

    ESP_LOGI(logtag, "*******************************************");
    ESP_LOGI(logtag, "Hello, this is Open Laser Tag Tagger.");
    ESP_LOGI(logtag, "Tagger Firmware version: ");
    ESP_LOGI(logtag, GIT_TAG);
    ESP_LOGI(logtag, "*******************************************");

    ESP_LOGD(logtag, "Starting init...");
    //create semaphores
    init_mutex();
    //trigger pin to interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_TRIGGER), handle_trigger, CHANGE); //LOW, CHANGE, RISING, FALLING
    //init bluetooth low energy server, services, characteristics
    init_ble();

    ESP_LOGD(logtag, "Enabling IRin...");
    irrecv_front.enableIRIn(); // Start the receiver
    //multiple receiver not working yet
    // irrecv_right.enableIRIn(); // Start the receiver
    // irrecv_left.enableIRIn(); // Start the receiver
    ESP_LOGD(logtag, "Enabled IRin");

    //init fast LED strip
    FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(LED_OVERALL_BRIGHTNESS);
    leds[LED_INDEX_BT].setColorCode(COLOR_BT_CONNECTION_OFF);
    FastLED.show();

    ESP_LOGD(logtag, "Init IR LED");
    ir_led.init(IR_RMT_TX_CHANNEL, IR_RMT_TX_GPIO_NUM);

    //init trigger and ir handling
    create_tasks();

    ESP_LOGD(logtag, "Entering team selection mode");
    team_selection = true;
    leds[LED_INDEX_TEAM].setColorCode(color_team[team]);
    FastLED.show();
    while (millis() - last_time_button_pressed_timestamp < TEAM_SELECTION_TIME_IN_MS)
    {
        //blink with bt status led while in team selection mode
        leds[LED_INDEX_BT].setColorCode(0);
        FastLED.show();
        vTaskDelay(200 / portTICK_PERIOD_MS);
        leds[LED_INDEX_BT].setColorCode(COLOR_BT_CONNECTION_OFF);
        FastLED.show();
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    ESP_LOGD(logtag, "Leaving team selection mode");
    team_selection = false;

    //blink for telling that setup is done
    ESP_LOGD(logtag, "Init finished: blink LED");
    led.blinks();
}

/**
 * @brief arduino loop
 * This is the arduino loop function. It is not used, instead RTOS tasks are used.
 * That's why the loop task is delayed max.
 * 
 */
void loop() { vTaskDelay(portMAX_DELAY); /*wait as much as possible ... */ }

/**
 * @brief Create all tasks object
 * This function creates all RTOS tasks. 
 */
void create_tasks()
{
    xTaskCreate(
        handle_ir,         /* Task function. */
        "handle_ir",       /* name of task. */
        2048,              /* Stack size of task */
        NULL,              /* parameter of the task */
        1,                 /* priority of the task */
        &xHandle_handle_ir /* Task handle to keep track of created task */
    );

    xTaskCreate(
        refresh_trigger_status,         /* Task function. */
        "refresh_trigger_status",       /* name of task. */
        2048,                           /* Stack size of task */
        NULL,                           /* parameter of the task */
        1,                              /* priority of the task */
        &xHandle_refresh_trigger_status /* Task handle to keep track of created task */
    );

    xTaskCreate(
        send_latency,         /* Task function. */
        "send_latency",       /* name of task. */
        2048,                 /* Stack size of task */
        NULL,                 /* parameter of the task */
        1,                    /* priority of the task */
        &xHandle_send_latency /* Task handle to keep track of created task */
    );

    xTaskCreate(
        handle_player_status,         /* Task function. */
        "handle_player_status",       /* name of task. */
        2048,                         /* Stack size of task */
        NULL,                         /* parameter of the task */
        1,                            /* priority of the task */
        &xHandle_handle_player_status /* Task handle to keep track of created task */
    );
}

void send_latency(void *parameter)
{
    const char *logtag = "latency";

    ESP_LOGD(logtag, "send latency task started");

    while (true)
    {
        //suspend until reactivated by onWrite of Ir_send_callbacks
        vTaskSuspend(NULL);
        //send latency via BT to app
        latency_char->setValue(latency);
        ble_notify(latency_char);
        ESP_LOGD(logtag, "latency sent via bt");
    }
}

/**
 * @brief create semaphores
 * Create mutex type semaphore for USB and BT communication.
 * Call this before using ble_notify or debug logger outputs.
 */
void init_mutex()
{
    const char *logtag = "initmutex";

    ESP_LOGD(logtag, "creating BT mutex...");
    xMutex_BT = xSemaphoreCreateMutex();
    if (xMutex_BT != NULL)
        ESP_LOGD(logtag, "BT Mutex successfully created.");
    else
        ESP_LOGE(logtag, "Could not create BT Mutex");
}