# Arduino libraries for Winsen gas sensors

These Arduino libraries handle various Winsen co. gas sensors.

Despite some sensors (e.g. ZE25-O3) offer also analog ouput, these libraries in general support only digital communication. This is due to diversity of development boards especially their ADCs and various input voltage ranges. While this would be possible to implement, it would sacrifice the simplicity of use which these libraries are aimed for (much more user input/configuration).

## Supported sensors
##### actually supported
- [ZE14-O3](lib/ZE27-O3)
- [ZE25-O3](lib/ZE14-O3)
- [ZE27-O3](lib/ZE22-O3)

##### work in progress
- MH-Z14A
- ZE15-CO
- ZE08-CH2O
- ZH06-I

## Documentation

Sensor specifics information are in respective sensors folders together with examples how the library can be used. For more information about individual sensor and datasheets refer to https://www.winsen-sensor.com

## Contribution

Contributions like bug fixes, improvements are always welcomed! Addition of new sensors is however limited due to the fact that I hesitate to add sensor which I cannot test on my own.

## Disclaimer

These libraries are for study purposes only!

Most of the gases these sensors detect are toxic when exceeding specific concetration. These sensors are not designed by Winsen for application where safety is concerned - nor these libraries are.

## About
GNU GPL v3 license, check LICENSE file for more information.

Written by Lukas Kurka
