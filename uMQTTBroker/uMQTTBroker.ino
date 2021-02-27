/*
 * uMQTTBroker demo for Arduino (C++-style)
 * 
 * The program defines a custom broker class with callbacks, 
 * starts it, subscribes locally to anything, and publishs a topic every second.
 * Try to connect from a remote client and publish something - the console will show this as well.
 */

#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
#include "SequenceTimer.h"
#include "MqttBroker.h"

/*
 * Your WiFi config here
 */
// Replace with your network credentials
const char* ssid = "SLAMET_MNTC 4119";
const char* password = "sampon170466";

const unsigned long BLINK_TIME = 1000;

bool WiFiAP = false;      // Do yo want the ESP as AP?

MqttBroker mqttBroker("Broker");
SequenceTimer   mainSequence("Sequence");

//for ESP environment, function should be declare first, 
//if not ERRORR !!!!! 
void startWiFiClient();
void startWiFiAP();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  Serial.begin(115200);

  // Start WiFi
  if (WiFiAP)
    startWiFiAP();
  else
    startWiFiClient();

  // Start the broker
  Serial.println("Starting MQTT broker");
  mqttBroker.init();

/*
 * Subscribe to anything
 */
  mqttBroker.subscribe("#");
}

int counter = 0;

void loop()
{
  mainSequence.execute();
  if (mainSequence.isASecondEvent()){
    digitalWrite( LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1 );

    /*
    * Publish the counter value as String
    */
    //mqttBroker.publish("broker/counter", (String)counter++);
  }
}

//Functions
/*
 * WiFi init stuff
 */
void startWiFiClient()
{
  Serial.println("Connecting to "+(String)ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void startWiFiAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}

