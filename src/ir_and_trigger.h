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

#define ERROR_MSG {0xA1, 0xF1, 0xFF, 0xFF, 0xFF}

void handle_ir(void * parameter);
void handle_trigger();
void refresh_trigger_status(void * parameter);
void irrecv_decode(IRrecv& irrecv);

#endif