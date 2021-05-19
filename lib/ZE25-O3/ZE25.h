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

#ifndef ZE25_h
#define ZE25_h

#include <Arduino.h>
#include <SoftwareSerial.h>

enum comm_type { kActiveUpload, kQuestionAnswer};

// handles Winsen ZE25-O3 sensor analog and serial readings
class ZE25 {
    private:
        // sensor messages and payloads configuration
        static const uint8_t kMessageLength = 9;
        const uint8_t kQuestionModePayload[kMessageLength] = {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};            // message which is send to sensor to set it to question/answer mode
        const uint8_t kActiveModePayload[kMessageLength] = {0xFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47};              // message which is send to sensor to set it to active upload mode
        const uint8_t kQuestionPayload[kMessageLength] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};                // message which is send to sensor to request the reading in QA mode
        
        // sensor communication configuration
        const uint16_t kSensorBaudRate = 9600;      // sensor built-in UART operates only with baud rate of 9600
        HardwareSerial* hw_stream_;
        SoftwareSerial* sw_stream_;
        Stream* stream_;

        // private processing variables
        float serial_ppm_;
        comm_type comm_type_;

        // identifies passed serial type HardwareSerial / SoftwareSerial and sets up appropriate stream for library
        void setStream();
        // returns calculated checksum of supplied sensor reading
        // ( calculation method =>> checksum=(negative(byte1+byte2+byte3+byte4+byte5+byte6+byte7))+1
        // byte0 (start byte) and obviously byte8 (checksum) are not include in the checksum )
        uint8_t calculateChecksum(uint8_t msg[kMessageLength]);

    public:
        // constructors takes as an argument appropriate serial port (HardwareSerial or
        // SoftwareSerial) and initialize it
        ZE25(HardwareSerial& device);
        // constructors takes as an argument appropriate serial port (HardwareSerial or
        // SoftwareSerial) and initialize it
        ZE25(SoftwareSerial& device);
        // instructs the object which communication mode to use. It accepts comm type argument
        // "kActiveUpload" or "kQuestionAnswer"
        void begin(comm_type communication_type);
        // switches the sensor to active upload mode by sending predefined payload
        // ( this member function is available for debugging only. It is used internally
        // and user don't have to call it)
        void setActiveMode();
        // switches the sensor to question/answer mode by sending predefined payload
        // ( this member function is available for debugging only. It is used internally
        // and user don't have to call it)
        void setQuestionMode();
        // asks the sensor to send data when in question/answer mode by sending predefined payload
        // - it has no effect in active upload mode. ( this member function is available for
        // debugging only. It is used internally and user don't have to call it)
        void requestReading();
        // reads the last reading from the sensor in active data upload mode, if successful it returns ppm in range 0-10.
        // if unsuccessful in returns negative number which is error code (-1) invalid reading (-2) min range exceeded
        // (-3) max range exceeded (-4) no message in buffer
        float getActiveData();
        // Reads the last reading from the sensor in question/answer mode,  if successful ppm is calculated and stored
        // in local class variable serial_ppm_
        float getQuestionData();
        // returns calculated PPM from sensor
        float getPPM();
};

#endif  // ZE25_h