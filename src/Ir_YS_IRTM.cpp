/**
 * @file Ir_YS_IRTM.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief class for IS_IRTM infrared transmitter module
 * @date 2019-01-23
 * 
 * 
 */

#include "Arduino.h"
#include "Ir_YS_IRTM.h"


Ir_YS_IRTM::Ir_YS_IRTM(uint8_t hardware_serial_nr, uint16_t baud_rate)
: HardwareSerial(hardware_serial_nr)
{
  //init ir with default baud rate
  begin(9600);
  //set custom baud rate
  if(baud_rate != 9600) set_baud_rate(baud_rate);
}

void Ir_YS_IRTM::send_ir(uint8_t data[]) {
  uint8_t ir_msg[5] = {0xA1, 0xF3, 0x00, 0x00, 0x00};

  //put three data bytes into ir message
  for (uint8_t i=0; i++; i<4) ir_msg[i+2] = data[i];

  //send ir message to ir module
  write(ir_msg,sizeof(ir_msg));  
}
  
void Ir_YS_IRTM::set_baud_rate_level(uint8_t baud_rate_level) {
  //set custom baud_rate
  uint8_t set_baud_rate_msg[5] = {0xA1, 0xF3, baud_rate_level, 0x00, 0x00};
  write(set_baud_rate_msg,sizeof(set_baud_rate_msg));

  //restart serial communication with custom baud rate
  end();
  if (baud_rate_level == 1) {
    _baud_rate = 4800;
  }
  else if (baud_rate_level == 2) {
    _baud_rate = 9600;
  }
  else if (baud_rate_level == 3) {
    _baud_rate = 19200;
  }
  else if (baud_rate_level == 4) {
    _baud_rate = 57600;
  }
  begin(_baud_rate); 
  return;
}

void Ir_YS_IRTM::set_baud_rate(uint16_t baud_rate) {
  if (baud_rate == 4800) set_baud_rate_level(1);
  else if (baud_rate == 9600) set_baud_rate_level(2);
  else if (baud_rate == 19200) set_baud_rate_level(3);
  else if (baud_rate == 57600) set_baud_rate_level(4);
  //else ; //TODO error handling?
  return;
}

uint16_t Ir_YS_IRTM::get_baud_rate() { return _baud_rate; }
