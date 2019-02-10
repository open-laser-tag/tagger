/**
 * @file ble.h
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief bluetooth low energy funcions
 * @date 2019-01-23
 * 
 * 
 */
#ifndef ble_h
#define ble_h

#include "tagger.h"

void init_ble();
void ble_notify(BLECharacteristic *characteristic);
extern bool device_connected;

#endif