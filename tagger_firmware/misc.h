/**
 * @file misc.h
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief small classes for tagger firmware
 * @date 2019-01-23
 *  
 */

#ifndef misc_h
#define misc_h

#include "Arduino.h"
#include <string>

class Led {
  public:
    Led           (int pin);
    void          light_on(),
                  light_off(),
                  blinks(int n=3, int duration_in_ms=150);
    
  private:
    int           _pin;
};



class Button {
  public:
    Button        (int pin);
    void          read_pin();
    bool          pressed = false;

  private:
    int           _pin;
};

class Logger: public HardwareSerial {
public:
  Logger          (
                  int hardware_serial_nr, // 0 for usb (rx_pin=3, tx_pin=1)
                  SemaphoreHandle_t *xSemaphore
                  ); 
  void            error(std::string msg),
                  warning(std::string msg),
                  info(std::string msg),
                  debug(std::string msg),
                  errorln(std::string msg),
                  warningln(std::string msg),
                  infoln(std::string msg),
                  debugln(std::string msg),
                  print_mutex(std::string msg);

private:
  SemaphoreHandle_t *_xSemaphore;
};

#endif
