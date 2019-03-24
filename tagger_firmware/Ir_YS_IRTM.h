/**
 * @file Ir_YS_IRTM.h
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief class for IS_IRTM infrared transmitter module
 * @date 2019-01-23
 * 
 */

#ifndef ir_stuff_h
#define ir_stuff_h

#include "Arduino.h"

class Ir_YS_IRTM : public HardwareSerial{
  public:
    Ir_YS_IRTM  (int hardware_serial_nr, int baud_rate=9600);   // 2 for rx_pin=16, tx_pin=17
    void        send_ir(uint8_t data[3]), //not tested
                set_baud_rate_level(uint8_t baud_rate_level), // 1 for 4800bps, 2 for 9600bps, 3 for 19200bps, 4 for 57600bps
                set_baud_rate(int baud_rate); //allowed values: 4800, 9600, 19200, 57600
    int         get_baud_rate();
                
    
  private:
    uint8_t     _set_baud_rate_msg[5];
    int         _baud_rate;
};


#endif
