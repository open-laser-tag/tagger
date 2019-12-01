/**
 * @file misc.cpp
 * @author gldhnchn (gldhnchn@posteo.de)
 * @brief small classes for tagger firmware
 * 
 */

#include "Arduino.h"
#include "misc.h"
#include <string>

/**
 * @brief reverse bit order of  byte
 * Reverses the bit order of one byte. Inspired by sth: https://stackoverflow.com/a/2602885/10114417
 * @param b one byte to reverse
 * @return unsigned char reversed byte
 */
uint8_t reverse_bit_order(uint8_t b)
{
    //First the left four bits are swapped with the right four bits.
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    //Then all adjacent pairs are swapped
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    //and then all adjacent single bits.
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

/**
 * @brief reverse bit order of unsinged long
 * Reverses the bit order of each byte of an unsigned long. Don't changes the order of the bytes.
 * @param ul one unsinged long (4 bytes) to reverse
 * @return unsigned long reversed unsinged long
 */
uint32_t reverse_bit_order(uint32_t ul)
{

    uint8_t b = 0;
    uint32_t ul_new = 0;

    for (int i = 0; i < 4 /*sizeof(uint32_t)/sizeof(uint8_t)*/; i++)
    {
        //use byte 0,1,2,3 of unsigned long
        b = ul >> i * 8 /*sizeof(uint8_t)*/; //8 is for 8 bits in one byte
        //reverse byte
        b = reverse_bit_order(b);
        //put reversed byte at old place in new unsigned long
        ul_new |= b << i * 8 /*sizeof(uint8_t)*/;
    }
    return ul_new;
}

Led::Led(uint8_t pin)
{
    pinMode(pin, OUTPUT);
    _pin = pin;
}

void Led::light_on()
{
    digitalWrite(_pin, 1);
}

void Led::light_off()
{
    digitalWrite(_pin, 0);
}

void Led::blinks(uint8_t n, uint16_t duration_in_ms)
{
    for (int i = 1; i <= n; i++)
    {
        light_on();
        delay(duration_in_ms);
        light_off();
        delay(duration_in_ms);
    }
}

Button::Button(uint8_t pin)
{
    pinMode(pin, INPUT_PULLUP);
    _pin = pin;
}

void Button::read_pin()
{
    pressed = !digitalRead(_pin);
    return;
}

Logger::Logger(uint8_t hardware_serial_nr, SemaphoreHandle_t *xSemaphore)
    : HardwareSerial(hardware_serial_nr)
{
    _xSemaphore = xSemaphore;
}

void Logger::error(std::string msg)
{
    msg.insert(0, "ERROR: ");
    print_mutex(msg);
    return;
}

void Logger::warning(std::string msg)
{
    msg.insert(0, "WARNING: ");
    print_mutex(msg);
    return;
}

void Logger::info(std::string msg)
{
    msg.insert(0, "INFO: ");
    print_mutex(msg);
    return;
}

void Logger::debug(std::string msg)
{
    msg.insert(0, "DEBUG: ");
    print_mutex(msg);
    return;
}

void Logger::errorln(std::string msg)
{
    msg.append("\r\n");
    error(msg);
    return;
}

void Logger::warningln(std::string msg)
{
    msg.append("\r\n");
    warning(msg);
    return;
}

void Logger::infoln(std::string msg)
{
    msg.append("\r\n");
    info(msg);
    return;
}

void Logger::debugln(std::string msg)
{
    msg.append("\r\n");
    debug(msg);
    return;
}

void Logger::print_mutex(std::string msg)
{
    //check if Mutex was successfully created
    if (*_xSemaphore != NULL)
    {
        /* See if we can obtain the semaphore.  If the semaphore is not
    available wait 10 ms to see if it becomes free. */
        if (xSemaphoreTake(*_xSemaphore, 10 / portTICK_PERIOD_MS) == pdTRUE)
        {
            /* We were able to obtain the semaphore and can now access the
        shared resource. */
            print(msg.c_str());
            /* We have finished accessing the shared resource.  Release the
        semaphore. */
            xSemaphoreGive(*_xSemaphore);
        }
        else
            println("Error: USB Mutex locked for over 10ms");
    }
    else
        println("Error: USB Mutex not available.");
}
