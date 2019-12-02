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
#include "strings.h"

#define ERROR_MSG 0xFFFF

void handle_ir(void *parameter);
void handle_trigger();
void refresh_trigger_status(void *parameter);
void irrecv_decode(IRrecv &irrecv);
bool msg_is_valid(uint32_t ir_recv_data);
void handle_player_status(void *parameter);

#endif