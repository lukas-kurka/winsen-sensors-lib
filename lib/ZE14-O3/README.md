# Winsen [ZE14-O3](https://www.winsen-sensor.com/sensors/o3-gas-sensor/ze14-o3.html) ozone sensor

This sensor has only active data upload mode so the reading should be called in
intervals no loger than 7s to prevent serial buffer overflow. If called less
often the object will return reading but it will not be the lastest one.

There is a possibility of a bug when using this sensor on ESP8266 with
SoftwareSerial which was observed with ZE25-O3 sensor. This bug causes that
first octet of message sent by the sensor may be missed by SoftwareSerial under
some unknown conditions. In such case the object correctly returns error. The use of
hardware serial is advised.