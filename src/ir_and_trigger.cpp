/**
 * @file ir_and_trigger.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief infrared stuff and trigger handler
 * @date 2019-01-23
 *
 */

#include "ir_and_trigger.h"

/**
 * @brief handle incoming infrared
 * This function reads the incoming infrared data and sends it via bluetooth.
 * @param parameter just a dummy parameter
 */
void handle_ir(void *parameter)
{
    usblog.debugln("handle IR task started");
    while (true)
    {
        uint32_t ir_data = 0;
        irrecv_decode(irrecv_front);
        // multiple receiver not working yet
        // irrecv_decode(irrecv_right);
        // irrecv_decode(irrecv_left);
        vTaskDelay(500 / portTICK_PERIOD_MS); //Block for 500ms.
    }
    return;
}

void irrecv_decode(IRrecv &irrecv)
{
    decode_results results;
    if (irrecv.decode(&results))
    {
        //the bits order changed after transmission, so it is reversed here
        uint32_t ir_recv_data = reverse_bit_order((uint32_t)results.value);
        usblog.info("Incoming IR: ");
        usblog.println(ir_recv_data, HEX);
        msg_nr++;
        usblog.info("message nr: ");
        usblog.println(msg_nr);
        if (msg_is_valid(ir_recv_data))
        {
            usblog.infoln("message valid");
            if (device_connected)
            {
                ir_receive_char->setValue((uint32_t &)ir_recv_data);
                ble_notify(ir_receive_char);
            }
            else
            {
                vTaskResume(xHandle_handle_player_status);
            }
        }
        else
            usblog.warningln("invalid message");
        irrecv.resume(); // Receive the next value
    }
    return;
}

/**
 * @brief check IR msg with inverted last byte
 * Checks whether the 4th byte is the 3rd byte inverted. Returns true when thats the case.
 */
bool msg_is_valid(uint32_t ir_recv_data)
{
    uint8_t third_byte = ir_recv_data >> 8,
            forth_byte = ~ir_recv_data;
    if (third_byte == forth_byte)
        return true;
    else
        return false;
}

void handle_trigger()
{
    last_bounce_time = xTaskGetTickCount();
    portENTER_CRITICAL_ISR(&mux);
    count_trigger_interrupts++;
    portEXIT_CRITICAL_ISR(&mux);
    vTaskResume(xHandle_refresh_trigger_status);
    return;
}

void refresh_trigger_status(void *parameter)
{
    usblog.debugln("refresh trigger status task started");
    while (true)
    {
        vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()
        //wait until the last bounce is longer ago than DEBOUNCETIME
        while (xTaskGetTickCount() - last_bounce_time < DEBOUNCETIME)
            vTaskDelay(10);
        //refresh trigger.pressed
        trigger.read_pin();
        usblog.info("Button Interrupt Triggered times: ");
        usblog.println(String(count_trigger_interrupts));
        usblog.info("time in ms since last trigger: ");
        usblog.println(String(xTaskGetTickCount() - last_bounce_time));
        usblog.info("trigger status: ");
        usblog.println(String(trigger.pressed));
        latency_timestamp = millis();
        if (device_connected)
        {
            usblog.debugln("sending trigger status via bt");
            trigger_char->setValue((int &)trigger.pressed);
            ble_notify(trigger_char);
        }
        else if (trigger.pressed && player_is_on)
        {
            usblog.infoln("Device not connected. Sending error message 0xFFFF via IR");
            ir_led.send(ERROR_MSG);
        }
        portENTER_CRITICAL_ISR(&mux);
        count_trigger_interrupts = 0;
        portEXIT_CRITICAL_ISR(&mux);
    }
}

void handle_player_status(void *parameter)
{
    usblog.debugln("handle player status task started");
    while (true)
    {
        player_is_on = true;
        leds[LED_INDEX_PLAYER_STATUS].setColorCode(COLOR_PLAYER_STATUS_ON);
        FastLED.show();
        vTaskSuspend(NULL); //suspend task until reactivated by handle_ir()
        player_is_on = false;
        leds[LED_INDEX_PLAYER_STATUS].setColorCode(COLOR_PLAYER_STATUS_OFF);
        FastLED.show();
        vTaskDelay(PLAYER_DOWNTIME_IN_MS);
    }
    return;
}