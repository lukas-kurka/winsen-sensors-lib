/* Arduino library for communication with various gas sensors by Winsen co.
Copyright (C) 2021 Lukas Kurka

This file is part of Winsen sensors library.

Winsen sensors library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Winsen sensors library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Winsen sensors library.  If not, see <https://www.gnu.org/licenses/>. */

#include <Arduino.h>
#include <ZE27.h>

ZE27 sensor(Serial2);                 // any uart can be used - Serial, Serial1, etc.

void setup() {
  Serial.begin(115200);
    sensor.begin(kActiveUpload);      // sets the sensor to active upload mode
}

void loop() {
  Serial.println(sensor.getPPM());    // prints out the ppm measured by the sensor (by default Serial.print displays float with two decimal which corresponds two sensor resolution)
  delay(1500);                        // if you would read the sensor at intervals less or equal to 1s there would be no message in buffer
}