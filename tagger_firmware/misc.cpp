/**
 * @file misc.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief small classes for tagger firmware
 * 
 */

#include "Arduino.h"
#include "misc.h"
#include <string>

Led::Led(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Led::light_on() {
  digitalWrite(_pin, 1);
}

void Led::light_off() {
  digitalWrite(_pin, 0);
}

void Led::blinks(int n, int duration_in_ms) {
  for (int i=1; i<=n; i++) {
    light_on();
    delay(duration_in_ms);
    light_off();
    delay(duration_in_ms);
  }
}

Button::Button(int pin) {
  pinMode(pin, INPUT_PULLUP);
  _pin = pin;
}

void Button::read_pin() {
  pressed = !digitalRead(_pin);
  return;
}

Logger::Logger(int hardware_serial_nr, SemaphoreHandle_t *xSemaphore)
: HardwareSerial(hardware_serial_nr)
{
  _xSemaphore = xSemaphore;
}

void Logger::error(std::string msg) {
  msg.insert (0, "ERROR: ");
  print_mutex(msg);
  return;
}

void Logger::warning(std::string msg) {
  msg.insert (0, "WARNING: ");
  print_mutex(msg);
  return;
}

void Logger::info(std::string msg) {
  msg.insert (0, "INFO: ");
  print_mutex(msg);
  return;
}


void Logger::debug(std::string msg) {
  msg.insert (0, "DEBUG: ");
  print_mutex(msg);
  return;
}

void Logger::errorln(std::string msg) {
  msg.append("\r\n");
  error(msg);
  return;
}

void Logger::warningln(std::string msg) {
  msg.append("\r\n");
  warning(msg);
  return;
}

void Logger::infoln(std::string msg) {
  msg.append("\r\n");
  info(msg);
  return;
}


void Logger::debugln(std::string msg) {
  msg.append("\r\n");
  debug(msg);
  return;
}

void Logger::print_mutex(std::string msg) {
  //check if Mutex was successfully created
  if( *_xSemaphore != NULL ) {
    /* See if we can obtain the semaphore.  If the semaphore is not
    available wait 10 ms to see if it becomes free. */
    if( xSemaphoreTake( *_xSemaphore, 10 / portTICK_PERIOD_MS ) == pdTRUE ) {
        /* We were able to obtain the semaphore and can now access the
        shared resource. */
        print(msg.c_str());
        /* We have finished accessing the shared resource.  Release the
        semaphore. */
        xSemaphoreGive( *_xSemaphore );
    }
    else println("Error: USB Mutex locked for over 10ms");
  }
  else println("Error: USB Mutex not available.");
}