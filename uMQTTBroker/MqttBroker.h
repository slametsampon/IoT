/*
  MqttBroker.h - Library for MQTT Broker ESP8266
  Dibuat Oleh : Sam Feb 27, 2021
*/
#ifndef MqttBroker_h
#define MqttBroker_h

#include "Arduino.h"

#include "uMQTTBroker.h"

class MqttBroker: public uMQTTBroker
{
  public:
    MqttBroker(String );
    virtual bool onConnect(IPAddress, uint16_t);
    virtual void onDisconnect(IPAddress, String);
    virtual bool onAuth(String, String, String);    
    virtual void onData(String, const char *, uint32_t);
    virtual void printClients();
  private:
    String _id;
};

#endif
