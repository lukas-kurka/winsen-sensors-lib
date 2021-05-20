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

#ifndef ZE14_h
#define ZE14_h

#include <Arduino.h>
#include <SoftwareSerial.h>

enum comm_type { kActiveUpload, kQuestionAnswer};

// handles Winsen ZE14-O3 sensor analog and serial readings
class ZE14 {
    private:
        // sensor messages configuration
        static const uint8_t kMessageLength = 9;
        
        // sensor communication configuration
        const uint16_t kSensorBaudRate = 9600;      // sensor built-in UART operates only with baud rate of 9600
        HardwareSerial* hw_stream_;
        SoftwareSerial* sw_stream_;
        Stream* stream_;

        // identifies passed serial type HardwareSerial / SoftwareSerial and sets up appropriate stream for library
        void setStream();
        // returns calculated checksum of supplied sensor reading
        // ( calculation method =>> checksum=(negative(byte1+byte2+byte3+byte4+byte5+byte6+byte7))+1
        // byte0 (start byte) and obviously byte8 (checksum) are not include in the checksum )
        uint8_t calculateChecksum(uint8_t msg[kMessageLength]);

    public:
        // constructors takes as an argument appropriate serial port (HardwareSerial or
        // SoftwareSerial)
        ZE14(HardwareSerial& device);
        // constructors takes as an argument appropriate serial port (HardwareSerial or
        // SoftwareSerial)
        ZE14(SoftwareSerial& device);
        // reads the last reading from the sensor in active data upload mode, if successful it returns ppm in range 0-10.
        // if unsuccessful in returns negative number which is error code (-1) invalid reading (-2) min range exceeded
        // (-3) max range exceeded (-4) no message in buffer
        float getActiveData();
        // returns calculated PPM from sensor
        float getPPM();
};

#endif  // ZE14_h