# Rhomeo Lights

Code that controls led strip lights for a camper van. The lights are a 12v RGBWW SK6812 driver LED strip. Lights are controlled via two encoders. One for brightness, one for hue.


## Required Components

* Two CYT1100 Encoders
* Simple push button
* Atleast one Waveshare 1.28inch 240x240 round LCD display. [Amazon Link](https://www.amazon.com/Waveshare-1-28inch-LCD-Module-Resolution/dp/B08V5538C6?&_encoding=UTF8&tag=floorplot-20)
* Microcontroller such as Arduino or ESP32


## Brightness Encoder

* Clicking encoder turns on light if current brightness == 0, turns off lights if current brightness is > 0
* Turning encoder clockwise increases brightness up to 100%.
* Turning encoder counterclockwise decreases brigghtness to 0%.

## Spectrum Encoder

* Turning encoder either direction cycles colors dynamically ranging from reds, blues, yellows, greens, purples, and more.

* Clicking Spectrum encoder resets color to warm white.

## Selector Button

* Selector Button cycles between different zones or groups of lightning. For example: Cabin Lights, Kitchen Cabinet Lights, Outdoor Awning Lights.

* Each group has seperate state for hue and brightness. Meaning it's possible to set cabin lights to 100% white, pressing the selector button, and setting awning lights to 50% red.

## Display

* State is shown via a UI indicating brightness as a semi-circle arc with a percentage. The arc is filled with the same color as the current spectrum color. The display also has a label for the current selected group that can be cycled with the selector button.

* Code should run on most microcontrollers supporting the Arduino Framework. In my case, it's running on an ESP32-WROOM-32D.