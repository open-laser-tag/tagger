#ifndef misc_h
#define misc_h

#include "Arduino.h"

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

#endif
