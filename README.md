<img src="docs/images/open-lasertag-logo.svg" alt="Open Lasertag Logo" width="100"/>

# Open Laser Tag
> because Laz00rs!

An cheap and DIY open source lasertag system, build with an ESP-32. Lasertag works with infrared. The tagger is build with a infrared LED and a infrared receiver, through which the players can hit each other. This basic functionality is given by the tagger and can be tested with two or more taggers standalone. For more complex games the esp32 is paired via bluetooth with a android app (work in progress). With the android app and a internet connection it will be possible to track statistics during a game and create more complex games e.g. with GPS location data.

This project currently consists of three parts:
* ESP-32 Firmware (https://github.com/open-laser-tag/tagger-firmware)
* Android App (https://github.com/open-laser-tag/android-app)
* node.js Server (https://github.com/open-laser-tag/server)

## Developing
clone repository with **submodules**
`git clone git@github.com:open-laser-tag/tagger.git --recurse-submodules`
There are a lot of options for writing code for the esp32 and for flashing it e.g. Arduino IDE, VScode with Arduino extention, PlattformIO. All possibilities to develop with the esp32 are listed and described [here](https://github.com/espressif/arduino-esp32/).

When uploading to esp32, don't forget to press IO0 on it when it tries to connect to the esp32.
![Button IO0 on ESP32 board](https://raw.githubusercontent.com/wiki/open-laser-tag/tagger/nodemcu_esp32_buttonio0.jpg)

When you want to keep track of which firmware is flashed on your tagger, you can add [update_git_hash.sh](https://github.com/open-laser-tag/tagger/blob/master/update_git_hash.sh) as git hook to your local git repository. Just add a file named `post-commit` to `.git/hooks/` and add the line `./update_git_hash.sh`.

### Arduino IDE
Prepare your IDE like described and then open
`tagger.ino`

`chose ESP32 DevModule` as Board
set partition scheme to `Minimal SPIFFS (Large `

### VScode with Arduino extention
Your arduino.json could look like this
```
{
    "port": "/dev/ttyUSB0",
    "board": "esp32:esp32:esp32",
    "configuration": "PSRAM=disabled,PartitionScheme=min_spiffs,CPUFreq=240,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=debug",
    "sketch": "tagger.ino",
    "output": "./build"
}
```

### PlatformIO - VScode
All necessery informations are already provided in platformio.ini. When your esp32 is connected via USB you can simply press upload.

### PlatformIO - CLI
When you have installed PlatformIO on your computer, you can just use the provided Makefile in this repository and run
`make upload` to download all dependencies, build the code, and upload it to your ESP-32 in one step.

### flashing with esptool.py and release binary
1. install [esptool.py](https://github.com/espressif/esptool) with their instructions
2. Download both .bin files from newest [release](https://github.com/open-laser-tag/tagger/releases/).
3. Flash ESP with `esptool.py --chip esp32 --port /dev/ttyUSB0 write_flash 0x10000 tagger_firmware.ino.bin 0x8000 tagger_firmware.ino.partitions.bin` (+ press Button IO0)

## Tagger Hardware
One tagger is made of: 1x ESP32, 1x IR LED [TSUS 5202](https://www.conrad.de/de/p/vishay-tsus-5202-cqw-13-ir-emitter-950-nm-15-5-mm-radial-bedrahtet-184551.html), 1x IR receiver [TSOP31238](https://www.segor.de/#Q=TSOP31238&M=1), 1x power bank, 1x micro usb cable, some RGB LEDS WS2812 or APA102, 1x [lense](https://www.ebay.de/itm/2x-Cardboard-Virtual-Reality-VR-BiConvex-Lenses-Only-25mm-x-45mm-OF-T-TPI/352821781036?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649), 1x [20 Ohm Resistor](https://www.segor.de/#Q=MF20R-1%2525&M=1), 1x [push button](https://www.segor.de/#/hilfe-zum-katalog). The price for all this parts is about 10€.

The pins for the wiring can be set or looked up in [tagger.h](https://github.com/open-laser-tag/tagger/blob/dev/src/tagger.h). Or look at the schematics below.
![Tagger schematics](https://github.com/open-laser-tag/tagger/blob/dev/docs/images/schematic.png)

## Licensing

This project is licensed under the terms of the GPL-3.0.
