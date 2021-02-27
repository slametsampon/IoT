
/*
  MqttBroker.h - Library for MQTT Broker ESP8266
  Dibuat Oleh : Sam Feb 27, 2021
*/

#include "MqttBroker.h"

MqttBroker::MqttBroker(String id):_id(id){}

bool MqttBroker::onConnect(IPAddress addr, uint16_t client_count) {
    Serial.println(addr.toString()+" connected");
    return true;
}

void MqttBroker::onDisconnect(IPAddress addr, String client_id) {
    Serial.println(addr.toString()+" ("+client_id+") disconnected");
}

bool MqttBroker::onAuth(String username, String password, String client_id) {
    Serial.println("Username/Password/ClientId: "+username+"/"+password+"/"+client_id);
    return true;
}

void MqttBroker::onData(String topic, const char *data, uint32_t length) {
    char data_str[length+1];
    os_memcpy(data_str, data, length);
    data_str[length] = '\0';
    
    Serial.println("received topic '"+topic+"' with data '"+(String)data_str+"'");
    //printClients();
}

// Sample for the usage of the client info methods

void MqttBroker::printClients() {
    for (int i = 0; i < getClientCount(); i++) {
    IPAddress addr;
    String client_id;
        
    getClientAddr(i, addr);
    getClientId(i, client_id);
    Serial.println("Client "+client_id+" on addr: "+addr.toString());
    }
}
