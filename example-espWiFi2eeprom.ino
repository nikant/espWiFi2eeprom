#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "espWiFi2eeprom.h"


// Update these with values suitable for your network.
// If you want static IP uncomment the following
//IPAddress nkip(192, 168, 1, 101); //Node static IP
//IPAddress nkgateway(192, 168, 1, 1);
//IPAddress nksubnet(255, 255, 255, 0);

ESP8266WebServer server(80);

String webPage1 = "<!DOCTYPE HTML>\n"
                  "<html><head><meta content=\"text/html;charset=utf-8\"><title>espWiFi2eeprom esp8266 example page</title>\n"
                  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
                  "</head>\n"
                  "<body>\n"
                  "<center>\n"
                  "<h1>espWiFi2eeprom</h1>"
                  "<h2>esp8266 example page</h2>\n";

String webPage2 =  "<br><hr>\n"
                   "</center>\n"
                   "</body></html>";


String webString = "";   // String to display


void handle_root() {
  // just something to output to webpage
  webString = "ESP Chip ID: " + String(ESP.getChipId()) + "<br>ESP Flash Chip ID: " + String(ESP.getFlashChipId()) + "<br>ESP Flash Chip Size: " + String(ESP.getFlashChipSize()) + "<br>ESP Free Heap: " + String(ESP.getFreeHeap());
  server.send(200, "text/html", webPage1 + webString + webPage2);
  delay(100);
}


void setup() {
  Serial.begin (57600);
  
  WiFi.persistent(false); // set WiFi credentials persistance to false to prevent eeprom corruption
  
  WiFi.setOutputPower(20.5);
  
  WiFi.mode(WIFI_STA);
  // uncomment the following if you set a static IP in the begining
  //WiFi.config(nkip, nkgateway, nksubnet);

  // call espWiFi2eeprom to connect to saved to eeprom AP or 
  // to create an AP to store new values for SSID and password
  espNKWiFiconnect();
  //--
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // espWiFi2eeprom: with the two following you can restart or clear eeprom
  // from your main esp8266 server
  server.on(restartcommand, handle_APrestart);
  server.on(cleareepromcommand, handle_clearAPeeprom);
  //--
  
  server.on("/", handle_root);

  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();
}
