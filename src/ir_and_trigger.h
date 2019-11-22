/**
 * @file ir_and_trigger.h
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief infrared stuff and trigger handler
 * @date 2019-01-23
 * 
 */
#ifndef ir_and_trigger_h
#define ir_and_trigger_h

#include "tagger.h"
#include <IRremote.h>
#include "esp_err.h"
#include "esp_log.h"

#define ERROR_MSG 0xFFFF

void handle_ir(void * parameter);
void handle_trigger();
void refresh_trigger_status(void * parameter);
void irrecv_decode(IRrecv& irrecv);
bool check_msg(uint32_t ir_recv_data);
bool lookup_msg(uint32_t *ir_recv_data);

#endif