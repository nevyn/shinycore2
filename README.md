# Shinycore2

Shinycore2 is the successor to [shinercore](https://github.com/nevyn/shinercore). It's the same thing, just a second try to make the code nicer, and targetting esp-idf instead of Arduino IDE.

Shinycore is an open source firmware for ESP32 controllers to make pretty animations on addressable RGB LED strips (such as NeoPixel strips). It's hard-coded to work well with M5Stack's M5AtomS3 modules, which are very small and cheap and work really great for the use case, but it shouldn't be too much of a pain to port it to another ESP32 platform.

The current feature set includes the ability to configure the animations over Bluetooth with an iOS app. The goal is to at beat syncing (having the animations blink to the beat of nearby music using a microphone peripheral); and to create a mesh network to sync animations with your friends, so you all pulse in time and with each others' colors.

**Shinycore2 is just freshly started. It doesn't work yet. Use [shinercore](https://github.com/nevyn/shinercore) meanwhile.**

## Usage

Supported hardware:

* [M5Stack Atom](https://shop.m5stack.com/products/atom-lite-esp32-development-kit) (just the basics)
* [M5Stack AtomS3](https://shop.m5stack.com/products/atoms3-lite-esp32s3-dev-kit) (faster)
* [M5Stack Atom Echo](https://shop.m5stack.com/products/atom-echo-smart-speaker-dev-kit) (with microphone, for audio reactive animations)
* [M5StickC PLUS](https://shop.m5stack.com/products/m5stickc-plus-esp32-pico-mini-iot-development-kit) (mic, screen, built-in battery. good for development.)

Then connect any addressable LED strip to the grove connector (both signal pins get the same animation).

Then [download the iOS app](https://apps.apple.com/se/app/shinercore-remote/id6451475618?l=en-GB
ShinerCore Remote) to control your LED installation.


## Compiling etc

1. [Install ESP-IDF, probably through the VScode extension](https://docs.espressif.com/projects/vscode-esp-idf-extension/en/latest/installation.html).
2. `idf.py build`
3. `idf.py flash monitor`
