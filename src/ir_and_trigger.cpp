/**
 * @file ir_and_trigger.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief infrared stuff and trigger handler
 * @date 2019-01-23
 *
 */

#include "ir_and_trigger.h"
#include "strings.h"
#include <IRremote.h>

/**
 * @brief handle incoming infrared
 * This function reads the incoming infrared data and sends it via bluetooth.
 * @param parameter just a dummy parameter
 */
void handle_ir(void * parameter) {

  usblog.debugln("handle IR task started");

  while(true) {

    uint32_t ir_data=0;
    irrecv_decode(irrecv_front);
    // multiple receiver not working yet
    // irrecv_decode(irrecv_right);
    // irrecv_decode(irrecv_left);

    vTaskDelay(500 / portTICK_PERIOD_MS); //Block for 500ms.
  }
  return;
}

void irrecv_decode(IRrecv& irrecv) {
  decode_results results;

  if (irrecv.decode(&results)) {
    //the bits order changed after transmission, so it is reversed here
    uint32_t ir_recv_data = reverse_bit_order((uint32_t)results.value);
    usblog.info("Incoming IR: ");
    usblog.println(ir_recv_data, HEX);
    msg_nr++;
    usblog.info("message nr: ");
    usblog.println(msg_nr);
    //TODO: move check und lookup to android app
    if (check_msg(ir_recv_data)) {
      usblog.infoln("message valid");
      ir_receive_char->setValue((uint32_t&)ir_recv_data);
      ble_notify(ir_receive_char);
    }
    else {
      usblog.infoln("message not valid, looking up if known");
      if(lookup_msg(&ir_recv_data)) {
        usblog.info("message known: ");
        usblog.println(ir_recv_data, HEX);
        ir_receive_char->setValue((uint32_t&)ir_recv_data);
        ble_notify(ir_receive_char);
      }
      else usblog.warningln("unknown message");
    }
    irrecv.resume(); // Receive the next value
  }
  return;
}

/**
 * @brief check IR msg with inverted last byte
 * Checks whether the 4th byte is the 3rd byte inverted. Returns true when thats the case.
 */
bool check_msg(uint32_t ir_recv_data) {
  uint8_t third_byte = ir_recv_data >> 8,
          forth_byte = ~ir_recv_data;

  if (third_byte == forth_byte) return true;
  else return false;
}

/**
 * @brief look up message whether a wrong message is known
 * This is a workaround function for the problem, that the transmission of a message fails very often and alternates between two values.
 */
bool lookup_msg(uint32_t *ir_recv_data) {
  //check if invalid message is known
  if(*ir_recv_data == 0x6DA80EFE) *ir_recv_data = 0xFFFFFF00;
  //look up wasn't successfull, so return false
  else return false;
  //look up was successfull, so return true
  return true;
}

void handle_trigger() {

  last_bounce_time = xTaskGetTickCount();

  portENTER_CRITICAL_ISR(&mux);
  count_trigger_interrupts++;
  portEXIT_CRITICAL_ISR(&mux);
  
  vTaskResume(xHandle_refresh_trigger_status);

  return;
}

void refresh_trigger_status(void * parameter) {

  usblog.debugln("refresh trigger status task started");
  
  while(true) {
    vTaskSuspend(NULL); //suspend task until reactivated by handle_trigger()

    //wait until the last bounce is longer ago than DEBOUNCETIME
    while (xTaskGetTickCount() - last_bounce_time < DEBOUNCETIME ) vTaskDelay(10);


    //refresh trigger.pressed
    trigger.read_pin();

    usblog.info("Button Interrupt Triggered times: ");
    usblog.println(String(count_trigger_interrupts));
    usblog.info("time in ms since last trigger: ");
    usblog.println(String(xTaskGetTickCount() - last_bounce_time));
    usblog.info("trigger status: ");
    usblog.println(String(trigger.pressed));
    latency_timestamp = millis();

    usblog.debugln("sending trigger status via bt");
    trigger_char->setValue((int&)trigger.pressed);
    ble_notify(trigger_char);
    if (!device_connected && trigger.pressed) {
      usblog.infoln("Device not connected. Sending error message 0xFFFF via IR");
      ir_led.send(ERROR_MSG);
    }

    portENTER_CRITICAL_ISR(&mux);
    count_trigger_interrupts = 0;
    portEXIT_CRITICAL_ISR(&mux);
  }
}
