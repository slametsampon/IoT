#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;

HTTPClient http;

WiFi.mode(WIFI_STA);
WiFiMulti.addAP("SSID", "PASSWORD");

// from example basicHttpClint.ino
void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID", "PASSWORD");

    //* https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/
    //! Unfortunately, by using the underlying Stream, we bypass the code that 
    //! handles chunked transfer encoding, so we must switch to HTTP version 1.0

    // Send request
    http.useHTTP10(true);
    http.begin("http://arduinojson.org/example.json");
    http.GET();

    // Parse response
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, http.getStream());

    /*
    ReadLoggingStream loggingStream(http.getStream(), Serial);
    deserializeJson(doc, loggingStream);  
    ? ReadLoggingStream will forward everything to deserializeJson() and will print the content to the serial port.  
    */

    // Read values
    Serial.println(doc["time"].as<long>());

    // Disconnect
    http.end();

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://jigsaw.w3.org/HTTP/connection.html")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(10000);
}
