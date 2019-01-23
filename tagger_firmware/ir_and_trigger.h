#ifndef ir_and_trigger_h
#define ir_and_trigger_h

#include "globals.h"

void handle_ir(void * parameter);
void handle_trigger();
void refresh_trigger_status(void * parameter);

#endif