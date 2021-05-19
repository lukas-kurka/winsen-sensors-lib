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

#include <ZE27.h>

void ZE27::setStream() {
    if (hw_stream_) {
        hw_stream_->begin(kSensorBaudRate);
        // while (!hw_stream_);
    } else {
        sw_stream_->begin(kSensorBaudRate);
        // while (!sw_stream_);
    }
    stream_ = !hw_stream_ ? (Stream*)sw_stream_ : hw_stream_;
}

uint8_t ZE27::calculateChecksum(uint8_t msg[kMessageLength]){
    uint8_t bytesum = 0;
    
    // sum of bytes 1 to 7
    for (uint8_t i = 1; i <= kMessageLength - 2; i++) {
        bytesum += msg[i];
    }
    // bitwise NOT + 1
    uint8_t checksum = (~bytesum)+1;
    return checksum;
}

ZE27::ZE27(HardwareSerial& device) : hw_stream_ (&device) {
    setStream();
}

ZE27::ZE27(SoftwareSerial& device) : sw_stream_(&device) {
    setStream();
}

void ZE27::begin(comm_type communication_type) {
    comm_type_ = communication_type;
    if (communication_type == kActiveUpload){
        setActiveMode();
    } else if (communication_type == kQuestionAnswer) {
        setQuestionMode();
    }
}

void ZE27::setActiveMode() {
    for (uint8_t i = 0; i < kMessageLength; i++) {
        stream_->write(kActiveModePayload[i]);
    } 
}

void ZE27::setQuestionMode(){
    for (uint8_t i = 0; i < kMessageLength; i++) {
        stream_->write(kQuestionModePayload[i]);
    } 
}

void ZE27::requestReading(){
    for (uint8_t i = 0; i < kMessageLength; i++) {
        stream_->write(kQuestionPayload[i]);
    } 
}

float ZE27::getActiveData() {
    uint8_t serial_buffer[64] = {0};                        // make sure the buffer array is always zeroed out
    uint8_t message_buffer[kMessageLength] = {0};           // make sure the reading array is always zeroed out

    // get last complete message from sensor
    int8_t counter = 0;
    if (stream_->available() >= kMessageLength) {           // make sure that there is at least one message in the buffer
        // read out the serial buffer to local array
        while (stream_->available() > 0) {
            serial_buffer[counter] = stream_->read();
            counter++;
        }
    
        // examine the buffer from the end and look for last complete message
        for (int8_t i = (sizeof(serial_buffer) - 1); i >= 0; i--) {
            // valid message starts with FF 2A octets and is 9 octets long
            if ((serial_buffer[i] == 0xFF) && (serial_buffer[i+1] == 0x2A) && ((i + kMessageLength - 1) <= static_cast<uint8_t>(sizeof(serial_buffer)))) {
                for (int8_t k = 0; k < kMessageLength; k++) {
                    message_buffer[k] = serial_buffer[i+k];
                }
                break;
            }
        }

        // if message seems to be valid calculate PPM and store it to local class variable
        if ((message_buffer[0] == 0xFF) && (message_buffer[1] == 0x2A)&& (message_buffer[8] == calculateChecksum(message_buffer))) {
            uint16_t ppb = (message_buffer[4] * 256) + message_buffer[5];
            float ppm = ppb / 1000.0;
            if (ppm > 10) {
                return -3;       // sensor max range exceeded
            } else if (ppm < 0) {
                return -2;       // sensor min range exceeded
            } else {
                return ppm;
            }
        } else {
            return -1;           // sensor reading invalid
        }
    } else {
        return -4;               // no message in buffer
    }
}

float ZE27::getQuestionData() {
    uint8_t serial_buffer[64] = {0};        // make sure the buffer array is always zeroed out

    // ask for the data and wait until recieved
    requestReading();
    delay(10);

    // read the data from serial buffer into local buffer
    uint8_t counter = 0;
    while (stream_->available() > 0) {
        serial_buffer[counter] = stream_->read();
        counter++;
    }

    // if message seems to be valid calculate and return PPM
    if ((serial_buffer[0] == 0xFF) && (serial_buffer[1] == 0x86)&& (serial_buffer[8] == calculateChecksum(serial_buffer))) {
        uint16_t ppb = (serial_buffer[2] * 256) + serial_buffer[3];
        float ppm = ppb / 1000.0;
        if (ppm > 10) {
            return -3;           // sensor max range exceeded
        } else if (ppm < 0) {
            return -2;           // sensor min range exceeded
        } else {
            return ppm;
        }
    }
    else return -1;              // sensor reading invalid
}

float ZE27::getPPM() {
    if (comm_type_ == kQuestionAnswer) {
        return getQuestionData();
    } else {
        return getActiveData();
    }
}