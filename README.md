<img src="docs/images/open-lasertag-logo.svg" alt="Open Lasertag Logo" width="100"/>

# Open Lasertag
> because Laz00rs!

An cheap and DIY open Source Lasertag build on the ESP-32 and Android. With an simple and expandable achitecture.
After breadboarding or soldering your ESP-32 together with an infraread Modul and glue a lense to it. You install the app on your phone and join a Free for all with all your friends.

Of course you can get as fancy as you want, 3D-printing your own case, enhancing the Tagger with LEDs...

This Project consists of thre parts:
* ESP-32 Firmware (https://github.com/open-lt/tagger-firmware)
* Android App (https://github.com/open-lt/android-app)
* node.js Server (https://github.com/open-lt/server)


## Developing

1. A computer with Arduino IDE configured to flash your ESP-32 see how to do this here (https://github.com/espressif/arduino-esp32/)
2. clone the tagger firmware repository (https://github.com/open-lt/tagger-firmware)
3. open it in your arduino IDE
4. flash it to your tagger

## Features

![Open lasertag tagger](/docs/images/open-lasertag-tagger-chart.svg)

* relay Data bewen Infra Red and Bluetooth LE


## Request for assistance
Idears are piling up. Come joine the forces!
Together we could continue the work with things like:
- 3D-Model for casing
- multiple hit zones

Don't hesitate to send pull request or contact us if you want to get involved.


## Licensing

This project is licensed under the terms of the GPL-3.0.