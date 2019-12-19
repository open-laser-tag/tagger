<img src="docs/images/open-lasertag-logo.svg" alt="Open Lasertag Logo" width="100"/>

# Open Laser Tag
> because Laz00rs!

An cheap and DIY open source lasertag system, build with an ESP-32 and an Android app. Our goal is a simple and expandable achitecture.
After breadboarding or soldering your ESP-32 together with an infraread module and gluing a lens to it, you can install the app on your phone and start a game with all your friends.

Of course you can get as fancy as you want, and e.g. 3D-printing your own case or enhance the tagger with LEDs. The sky
is the limit...

This project currently consists of three parts:
* ESP-32 Firmware (https://github.com/open-laser-tag/tagger-firmware)
* Android App (https://github.com/open-laser-tag/android-app)
* node.js Server (https://github.com/open-laser-tag/server)


## Developing
clone repository with **submodules**
`git clone git@github.com:open-laser-tag/tagger.git --recurse-submodules`

run `$ ./update_git_hash.sh` to make the version hash accessible


### Arduino IDE
Prepare your IDE like described [here](https://github.com/espressif/arduino-esp32/) and then open
`src/tagger_firmware.ino`

`chose ESP32 DevModule` as Board
set partition sceme to `Minimal SPIFFS (Large `

### Platformio
When you have installed PlatformIO on your computer, you can just use the provided Makefile in this repository and run
`make upload` to download all dependencies, build the code, and upload it to your ESP-32 in one step.


## Features

![Open lasertag tagger](/docs/images/open-lasertag-tagger-chart.svg)


## Request for assistance
Ideas are piling up. Come join the forces!
Together we could continue the work with things like:

- 3D-Model for a case
- Multiple hit zones

Don't hesitate to send pull request or contact us if you want to get involved.


## Licensing

This project is licensed under the terms of the GPL-3.0.
