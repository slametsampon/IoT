// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License
//
// This example shows the different ways you can use String with ArduinoJson.
//
// Use String objects sparingly, because ArduinoJson duplicates them in the
// JsonDocument. Prefer plain old char[], as they are more efficient in term of
// code size, speed, and memory usage.
//
// https://arduinojson.org/v6/example/string/

#include <ArduinoJson.h>

void setup() {
    Serial.begin(115200);

  // from https://arduinojson.org/v6/assistant/
  StaticJsonDocument<192> doc;

  doc["id"] = "Smoke-1";
  doc["unit"] = "%";
  doc["indexMenu"] = 1;
  doc["value"] = 51.5;
  doc["highRange"] = 100;
  doc["lowRange"] = 0;
  doc["highLimit"] = 80;
  doc["lowLimit"] = 40;
  doc["increment"] = 1.1;
    // Lastly, you can print the resulting JSON to a String
    String output;

    serializeJson(doc, output);

    Serial.print("Output : ");
    Serial.println(output);

    Serial.print("Json Doc : ");
    serializeJson(doc, Serial);

    // Get a reference to the root object
    JsonObject obj = doc.as<JsonObject>();

    // Loop through all the key-value pairs in obj
    Serial.println("\nKey - Value Pair");
    String str;
    for (JsonPair p : obj) {
      str = p.key().c_str();
      str = String (str + " : ");
      str = String (str + p.value().as<String>());
      Serial.println(str);
    }    
  }

void loop() {
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any problem.
//
// The book "Mastering ArduinoJson" contains a quick C++ course that explains
// how your microcontroller stores strings in memory. On several occasions, it
// shows how you can avoid String in your program.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
