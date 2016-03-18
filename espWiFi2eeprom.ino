/*
"esp8266 WiFi configuration to eeprom manager for Arduino IDE"

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

Check example-espWiFi2eeprom.ino for an example of usage along with another server.

You can edit espWiFi2eeprom.h in order to change parameters.

--------------------------------------------------------------------------------
*/



#include <ESP8266WiFi.h>
#include <ESP8266Webserver.h>
#include <EEPROM.h>

const char* restartcommand = "/" AP_RESTART;
const char* cleareepromcommand = "/" AP_CLEAREEPROM;

ESP8266WebServer APserver(AP_WIFICFGPORT);

String APwebPage1 = "<!DOCTYPE HTML>\n"
                    "<html><head><meta content=\"text/html;charset=utf-8\"><title>esp8266 WiFi setup control</title>\n"
                    "<style type=\"text/css\">body { font-family: sans-serif; background-color: #000; color: #fff; font-size:1.2em; } a:link, a:visited, a:hover, a:active { color:#fff; }</style>\n"
                    "</head>\n"
                    "<body>\n"
                    "<center>\n"
                    "<h1>esp8266 WiFi setup control</h1>\n<br>"
                    "<table style=\"width:100%;border: 1px solid #fff;\"><tr>"
                    "<th style=\"text-align:center;width:50%;\"><form action='/APsubmit' method='POST'><input type=\"text\" name=\"newssid\" id=\"formnewssid\" value=\"\"><br><input type=\"text\" name=\"newpass\" value=\"\" size=\"32\" maxlength=\"64\"><br><input type=\"submit\" value=\"Submit\"></form></th>"
                    "<th style=\"text-align:left;width:50%;\">";

String APwebPage2 =  "</th></tr></table>\n"
                     "<br><br><form action=\"/\" target=\"_top\"><input type=\"submit\" value=\"home / rescan networks\"></form>\n"
                     "<br><br><form action=\"" + String(restartcommand) + "\" target=\"_top\"><input type=\"submit\" value=\"restart esp8266\"></form>\n"
                     "<br><br><form action=\"" + String(cleareepromcommand) + "\" target=\"_top\"><input type=\"submit\" value=\"-* clear EEPROM *-\"></form>\n"
                     "<br><br><b>- version: " ESPWIFI2EEPROM_VERSION " -</b></a>\n"
                     "</center>\n"
                     "</body></html>";


String APwebstring = "";   // String to display


void handle_clearAPeeprom() {
  Serial.println("-* Clearing eeprom *-");
  for (int i = 0; i < 96; ++i) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  delay(1000);
  ESP.restart();
}

void handle_APrestart() {
  Serial.println("-* Restarting in 1 sec! *-");
  delay(1000);
  ESP.restart();
}

void handle_AProot() {
  getAPlist();
  APserver.send(200, "text/html", APwebPage1 + APwebstring + APwebPage2);
  delay(100);
}

void handle_APsubmit() {
  String thenewssid = APserver.arg("newssid");
  String thenewpass = APserver.arg("newpass");
  if (thenewssid != "") {
    Serial.println("-* Clearing eeprom *-");
    for (int i = 0; i < 96; ++i) {
      EEPROM.write(i, 0);
    }

    Serial.println("Writing SSID to EEPROM");
    for (int i = 0; i < thenewssid.length(); ++i)
    {
      EEPROM.write(i, thenewssid[i]);
      //Serial.print("Wrote: ");
      //Serial.println(thenewssid[i]);
    }
    Serial.println("Writing password to EEPROM");
    for (int i = 0; i < thenewpass.length(); ++i)
    {
      EEPROM.write(32 + i, thenewpass[i]);
      //Serial.print("Wrote: ");
      //Serial.println(thenewpass[i]);
    }

    if (EEPROM.commit()) {
      APwebstring = "<h2><b><p>Saved to eeprom... restart to boot into new wifi</b></h2>\n";
    } else {
      APwebstring = "<h2><b><p>Couldn't write to eeprom. Please try again.</b></h2>\n";
    }
    delay(10);


    APserver.send(200, "text/html", APwebPage1 + APwebstring + APwebPage2);
  }
}

String printConnectionType(int thisType) {
  String con_type = "";
  // read connection type and print out the name:
  switch (thisType) {
    case 255:
      return con_type = "WL_NO_SHIELD";
    case 0:
      return con_type = "WL_IDLE_STATUS";
    case 1:
      return con_type = "WL_NO_SSID_AVAIL";
    case 2:
      return con_type = "WL_SCAN_COMPLETED";
    case 3:
      return con_type = "WL_CONNECTED";
    case 4:
      return con_type = "WL_CONNECT_FAILED";
    case 5:
      return con_type = "WL_CONNECTION_LOST";
    case 6:
      return con_type = "WL_DISCONNECTED";
    default:
      return con_type = "?";
  }
}

boolean testWiFi() {
  int c = 0;
  Serial.print("Waiting for Wifi to connect");
  while ( c < 60 ) {
    Serial.print(".");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.println(printConnectionType(WiFi.status()));
      return 1;
    }
    delay(500);
    c++;
  }
  Serial.println("");
  Serial.println(printConnectionType(WiFi.status()));
  return 0;
}

String printEncryptionType(int thisType) {
  String enc_type = "";
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      return enc_type = "WEP";
    case ENC_TYPE_TKIP:
      return enc_type = "WPA";
    case ENC_TYPE_CCMP:
      return enc_type = "WPA2";
    case ENC_TYPE_NONE:
      return enc_type = "None";
    case ENC_TYPE_AUTO:
      return enc_type = "Auto";
    default:
      return enc_type = "?";
  }
}

void getAPlist() {
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    Serial.println("No networks found :(");
    APwebstring = "No networks found :(";
    return;
  }

  // sort by RSSI
  int indices[n];
  for (int i = 0; i < n; i++) {
    indices[i] = i;
  }
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
        std::swap(indices[i], indices[j]);
      }
    }
  }

  Serial.println("");
  APwebstring = "<ul>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    APwebstring += "<li>";
    APwebstring += i + 1;
    APwebstring += ":&nbsp;&nbsp;<b>";
    APwebstring += "<a href=\"#\" target=\"_top\" onClick=\"document.getElementById(\'formnewssid\').value=\'" + WiFi.SSID(indices[i]) + "\'\">";
    APwebstring += WiFi.SSID(indices[i]);
    APwebstring += "</a>";
    APwebstring += "</b>&nbsp;&nbsp;&nbsp;(";
    APwebstring += WiFi.RSSI(indices[i]);
    APwebstring += "&nbsp;dBm)&nbsp;&nbsp;&nbsp;";
    APwebstring += printEncryptionType(WiFi.encryptionType(indices[i]));
    APwebstring += "</li>";
  }
  APwebstring += "</ul>";
  delay(100);
}


void setupWiFiAP() {
  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd):
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "esp8266 control " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  getAPlist();

  APserver.on("/", handle_AProot);
  APserver.on("/APsubmit", handle_APsubmit);
  APserver.on(restartcommand, handle_APrestart);
  APserver.on(cleareepromcommand, handle_clearAPeeprom);

  WiFi.softAP(AP_NameChar, AP_password);

  APserver.begin();

  Serial.print("SoftAP IP address: ");
  Serial.println(WiFi.softAPIP());

  while (WiFi.status() != WL_CONNECTED) {
    APserver.handleClient();
  }
}


void espNKWiFiconnect() {

  WiFi.mode(WIFI_STA);

  EEPROM.begin(512);
  Serial.println("");
  // read eeprom for ssid and pass
  Serial.println("Reading EEPROM SSID");
  String esid = "";
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  //Serial.print("SSID: ");
  //Serial.println(esid);
  Serial.println("Reading EEPROM password");
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  //Serial.print("PASS: ");
  //Serial.println(epass);

  if (esid != "") {
    // test esid
    WiFi.begin(esid.c_str(), epass.c_str());

    boolean testWiFiAP = testWiFi();

    if (testWiFiAP) {
      return;
    } else if (!testWiFiAP) {
      Serial.println("Could not connect to SSID!");
      if (WiFi.status() == WL_CONNECT_FAILED) {
        Serial.println("Connection failed! Maybe SSID/password are wrong.");
        Serial.println("Clearing EEPROM and restarting!");
        delay(500);
        handle_clearAPeeprom();
        delay(100);
        handle_APrestart();
      } else {
        Serial.println("Starting AP at port: " + String(AP_WIFICFGPORT));
        setupWiFiAP();
      }
    }
  } else {
    Serial.println("SSID empty! Starting AP at port: " + String(AP_WIFICFGPORT));
    setupWiFiAP();
  }
}

