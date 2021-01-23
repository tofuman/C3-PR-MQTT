#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include <WiFi.h>

class Config;

class MqttClient {
    public:
        MqttClient():wifiClient(),mqttClient(wifiClient){};
        bool setup(Config* config);
        void tick();

    private:
        WiFiClient wifiClient;
        PubSubClient mqttClient;
        char mqttServer[30];
        int mqttPort;

        bool reconnectMQTT(int retry);
};

#endif