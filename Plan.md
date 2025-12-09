# Shinycore 2 implementation plan

Shinycore is an open source firmware for ESP32 controllers to make pretty animations on addressable RGB LED strips (such as NeoPixel strips). The current feature set includes the ability to configure the animations over Bluetooth with an iOS app, and this animation is then automatically played onto the two signal pins of the grove connector of an M5Atom or similar.

# Data model

## Original
Shinycore 1 had the following data model. The data model is both reflected in storage on disk, and in the bluetooth protocol. All values are stored and transmitted as strings. If the documentation below says it's a number, it's actually a string with that number.

Device global properties:
* The BLE service itself, with UUID `6c0de004-629d-4717-bed5-847fddfbdc2e`
* Name of the device, string, characteristic `7ad50f2a-01b5-4522-9792-d3fd4af5942f`
* Mode, which used to be an index of animations where 0 is off, but now is just 0 for off and 1 for on. Characteristic `70d4cabe-82cc-470a-a572-95c23f1316ff`.

Animation properties:
* Speed, `5341966c-da42-4b65-9c27-5de57b642e28`. The duration of a full animation cycle, in seconds.
* Brightness, `2B01`. Integer 0-255 of how bright to display this animation.
* Color 1, `c116fce1-9a8a-4084-80a3-b83be2fbd108`, as a string with three numbers 0-255 for R G and B. The first color of the animation.
* Color 2, `83595a76-1b17-4158-bcee-e702c3165caf`. Same format as color 1.
* Tau, `d879c81a-09f0-4a24-a66c-cebf358bb97a`, a floating point value that adjusts a parameter in the animation.
* Phi, `df6f0905-09bd-4bf6-b6f5-45b5a4d20d52`, a second floating point value adjusting the animation.

## Extension in Shinycore 2

Shinycore 2's data model and protocol should be backwards compatible over the air, so that the same ios app is practically compatible. Shiny 2 adds give parameters:

* Pattern
* Layer
* Blend mode
* Preset
* Strip type

* Pattern, `bee29c30-aa11-45b2-b5a2-8ff8d0bab262`, is straightforward. A string with the name of an animation pattern, such as "double sine", "fade", etc. This is what "mode" was before, except now as a string.
* Layer, `0a7eadd8-e4b8-4384-8308-e67a32262cc4`, is an integer 0-9 indicating the current animation layer. 
    * Layers compose to create an animation. If you change this number, every animation property (speed, brightness, colors, tau, phi, pattern and blend mode) will publish new values to match the settings for this layer. 
    * During animation, every layer that has settings is composed together using "blend mode". 
    * For example, if layer 1 has "sine wave" animation with a red wave moving to the right, and layer 2 has "fade" animation between black and white and with the blend mode "multiply", the final animation will show a red wave moving to the right that also as a while fades in and out.
* Blend mode, `03686c5c-6e6f-44f0-943f-db6388d9fdd4` is how each layer should compose on top of the previous one. 
    * "set" clears out whatever is on the strip with the new value. This is the default  blend mode of layer 0.
    * "add" adds the values of the current layer's animation to the current value currently on the strip, capping at 255.
    * "subtract" removes, capping at 0
    * "add-wrap" adds, but overflows and carries over. E g, 250 + 10 = 5
    * "subtract-wrap" underflows
    * "multiply" maps the range 0-255 to 0.0-1.0 of both strip and layer animation value, and then multiplies them together, and then maps back and writes back to the strip
    * "average" averages current and layer's values
* Preset, `8b989f5e-3d22-4377-80c9-c54eeb459518`, is one more abstraction on top of "Layer", and is also an integer 0-9. Each preset contains a full set of layers. This is so each Shinycore can contain multiple programmed presets that you can then switch between with the button on it.
    * Changing preset will thus republish all of the animation properties to show and allow for editing the current layer in the new preset.
* Strip type, the kind of addressable LED strip it is. Space separated string of:
    * standard (neopixel, dotstar)
    * color order (rgb, bgr, rgbw, etc)

# Plan

In this ESP-IDF project, targetting the esp32 platform M5Stack Atom Lite, preferrably without using M5Stack's own shitty libraries:

- [ ] Get a "hello world" project up and running, which blinks the one addressable rgb led on an M5Atom on pin 27
    - [ ] Pick a good RGB LED library, which supports a wide range of addressable LED strips. Different standards (neopixel, dotstar); different layouts (rgb, bgr, rgbw etc); and whatever other things would be nice to support.
- [ ] Implement a data model. The data model itself should be POD structs without functionality, and then helper functions to modify them in meaningful ways.
    - [ ] Struct that represents the full CoreState, with name and mode and an array of Presets.
    - [ ] Struct that represents a Preset, with an array of layers
    - [ ] Struct that represents a Layer, with all of the animation properties on it
- [ ] Implement serialization so that the full CoreState is stored persistently on the esp32 and restores after reboot.
- [ ] Implement an animation system
    - [ ] Keep track of time as a double representing seconds-since-boot
    - [ ] Implement animation pattern functions, and keep them in a dictionary mapping string name to function pointer
        * Each function should take a constref to the layer it's implementing (so it can read colors etc), time, and the destination array to fill with numbers
    - [ ] Implement animation function
        - [ ] Make it be called 100 times-ish per second
        - [ ] loops through all the layers in the current preset
        - [ ] For each layer, use a temp array and fill it with data using the correct animation pattern for that layer
        - [ ] Blend the temp array into the strip's current state using the current layer's blend mode
- [ ] Implement the BLE protocol, using some clever mapping from structs to BLE characteristics
    - [ ] Make sure to re-publish whenever any animation property is changed 
    - [ ] and republish all the animation properties if layer or preset is changed
