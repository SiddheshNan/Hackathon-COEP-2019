#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "SmartHelmetAP";
const char *password = "12345678";
const char *host = "192.168.4.1";

int inputPin = 4;
//int RledPin = 5;
String pinStatus;
String donem;


void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(inputPin, INPUT_PULLUP);
  //pinMode(RledPin, OUTPUT);
  //digitalWrite(RledPin, HIGH);

  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (digitalRead(inputPin) == 0) {
    donem = "0";
    pinStatus = "ghatla";
    Serial.println("ghatla");
    delay(10);
    doGetReq();
  }
  if (digitalRead(inputPin) == 1) {
    donem = "1";
    pinStatus = "nahi-ghatla";
    Serial.println("nahi-ghatla");
    delay(10);
    doGetReq();
  }

}


void loop() {


  if ((digitalRead(inputPin) == 0) && (donem == "1")) {
    donem = "0";
    pinStatus = "ghatla";
    Serial.println("ghatla");
    delay(10);
    doGetReq();
  }


  else if ((digitalRead(inputPin) == 1) && (donem == "0")) {
    donem = "1";
    pinStatus = "nahi-ghatla";
    Serial.println("nahi-ghatla");
    delay(10);
    doGetReq();
  }

}

void doGetReq(void) {

  HTTPClient http;

  String Link = "http://192.168.4.1/" + pinStatus;

  http.begin(Link);

  int httpCode = http.GET();
  String payload = http.getString();

  Serial.println("HTTP Code:");
  Serial.println(httpCode);
  Serial.println("Payload:");
  Serial.println(payload);

  http.end();

  delay(50);

}
