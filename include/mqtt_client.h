#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include <WiFi.h>

#include "mqtt_message.h"

class Config;

class MqttClient {
    public:
        MqttClient():wifiClient(),mqttClient(wifiClient){};
        bool setup(Config *config);
        bool subscribe(char* topic); 
        bool publish(char* topic, char* message);
        void tick();
        int get_messages_pending();
        Message *get_message();


    private:
        WiFiClient wifiClient;
        PubSubClient mqttClient;
        char mqttServer[30];
        int mqttPort;
        Config *config;

        bool reconnectMQTT(int retry);
};

#endif