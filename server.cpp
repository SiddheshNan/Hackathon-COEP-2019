#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

IPAddress apIP(192, 168, 4, 1);

const char *ssid = "SmartHelmetAP";
const char *password = "12345678";

ESP8266WebServer server(80);

String donem;
String helmet;

int relay = 4;

char outText;

bool relayState = false;

void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  delay(100);
  WiFi.mode(WIFI_OFF);
  delay(500);
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  helmet = "nahi-ghatla";
  relayState = false;

  server.on ( "/", handleRoot );
  server.on ( "/ghatla", handleRootGhetla);
  server.on ( "/nahi-ghatla", handleRootNahiGhetla);

  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println("HTTP server started");
}


void handleRoot() {
  char outText[80];
  char html[1000];
  if (relayState) {
    strcpy(outText, "ghatla");
  }
  else {
    strcpy(outText, "nahi-ghatla");
  }
  snprintf ( html, 1000,
"<html>\
  <head>\
    <meta http-equiv='refresh' content='1'/>\
    <title>Smart Helmet</title>\
    <style>.centerText{padding: 200px 0;text-align: center;color: #E74C3C;}</style>\
  </head>\
<body><h1 class='centerText'>Helmet : %s</h1></body>\
</html>",outText);
  server.send ( 200, "text/html", html );
}

void handleRootGhetla() {
  char outText[80];
  char html[1000];
  donem = "0";
  digitalWrite(relay, LOW);
  Serial.println("Received from HTTP-GET:");
  Serial.println("ghatla");
  relayState = true;
  delay(50);
  snprintf ( html, 1000, "Success! Helmet:Ghatla");
  server.send ( 200, "text/html", html );
}

void handleRootNahiGhetla() {
  char outText[80];
  char html[1000];
  donem = "1";
  digitalWrite(relay, HIGH);
  Serial.println("Received from HTTP-GET:");
  Serial.println("nahi-ghatla");
  relayState = false;
  delay(50);
  snprintf ( html, 1000, "Success! Helmet:NahiGhatla");
  server.send ( 200, "text/html", html );
}

void loop() {
  server.handleClient();
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.send ( 404, "text/plain", message );
}
