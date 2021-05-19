# Winsen [ZE25-O3](https://www.winsen-sensor.com/sensors/o3-gas-sensor/ze25-o3.html) ozone sensor

In active data upload the sensor reading should be called in intervals no loger
that 7s to prevent serial buffer overflow. If called less often the object will
return reading but it will not be the lastest one. If needed to read the sensor
less often use question/answer mode which is recommend anyway.

There is a known bug when using this sensor on ESP8266 with SoftwareSerial. It
causes that first octet of message sent by the sensor may be missed under some
unknown conditions. In such case the object correctly returns error. The use of
hardware serial is advised.

(this is actaully the same library as for ZE27-O3, the only difference between
those two is that the one has connector and the other header)