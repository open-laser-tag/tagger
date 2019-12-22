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
There are a lot of options for writing code for the esp32 and for flashing it e.g. Arduino IDE, VScode with Arduino extention, PlattformIO. All possibilities to develop with the esp32 are listed and described [here](https://github.com/espressif/arduino-esp32/)

### Arduino IDE
Prepare your IDE like described and then open
`tagger.ino`

`chose ESP32 DevModule` as Board
set partition scheme to `Minimal SPIFFS (Large `

### VScode with Arduino extention
Your arduino.json could look like this
```
{
    "port": "/dev/USB0",
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

## Licensing

This project is licensed under the terms of the GPL-3.0.
