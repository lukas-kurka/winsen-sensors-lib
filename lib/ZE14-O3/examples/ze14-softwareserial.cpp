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
#include <SoftwareSerial.h>
#include <ZE14.h>

SoftwareSerial sw_serial(0, 4);             // replace the pin numbers to match yours
ZE14 sensor(sw_serial);

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print(sensor.getPPM(), 1);         // print only one decimal place as that is the resolution of sensor
  Serial.println(" ppm");
  delay(1500);                              // if you would read the sensor at intervals less or equal to 1s there would be no message in buffer
}