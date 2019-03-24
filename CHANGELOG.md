# v0.0.4
* add sending error message 0xFFFFFF via IR when no BT connected
* add support for two more ir receiver
* add support for ws2812 led strip

# v0.0.3
* fixed bit order in received data from tsop ir reciever

# v0.0.2
* tagger firmware version readable via bluetooth
* new additional ir receiver with irremote.h lib
* add semaphores for handling communication rescources
* improve usb debug output

# v0.0.1
* first version of tagger firmware with trigger, infrared and bluetooth
* esp32
* freertos
* bluetooth low energy
* debounced trigger
* infrared with ir module YS-IRTM 
* latency between pressed trigger and ir shot is sent via bluetooth
