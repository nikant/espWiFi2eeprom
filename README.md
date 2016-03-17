# esp8266 WiFi configuration to eeprom manager for Arduino IDE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

 - idea and some code parts from chriscook8 esp-arduino-apboot https://github.com/chriscook8/esp-arduino-apboot
 - sort by RSSI by tablatronix https://github.com/esp8266/Arduino/issues/1355
 - thanks goes to fivosv for his patience to help me
 
----------------

-* THIS IS NOT A LIBRARY (and it may never become one..) *-

You must have the two files "espWiFi2eeprom.h" and "espWiFi2eeprom.ino" in the same directory with your project sketch.

-* Has been tested with NodeMCU v1.0 and Arduino IDE 1.6.5-r5 in Windows 7 *-

Check example-espWiFi2eeprom.ino for an example of usage along with another server.

You can edit espWiFi2eeprom.h in order to change parameters.

--------------------------------------------------------------------------------
