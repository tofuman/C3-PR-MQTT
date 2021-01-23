
#include "string.h"

#include "config.h"
#include "mqtt_client.h"
// MQTT client
static void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

bool MqttClient::setup(Config *config)
{

    strcpy(this->mqttServer, config->get_mqtt_broker_ip());
    mqttPort = config->get_mqtt_broker_port();

    mqttClient.setServer(mqttServer, mqttPort);
    Serial.print("MQTT Client ");
    Serial.print(mqttServer);
    Serial.print(":");
    Serial.print(mqttPort);
    Serial.print("\n");
    // set the callback function
    mqttClient.setCallback(callback);

    return reconnectMQTT(1);
}

bool MqttClient::reconnectMQTT(int retry)
{
    int trys = 0;
    while (!mqttClient.connected() && trys < retry)
    {
        Serial.println("Reconnecting to MQTT Broker..");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);

        if (mqttClient.connect(clientId.c_str()))
        {
            Serial.println("Connected.");
            // subscribe to topic
            mqttClient.publish("keepAlive/abc", "hello world");
            // ... and resubscribe
            mqttClient.subscribe("cmd/abc");
        }
        trys++;
    }
    if (!mqttClient.connected())
    {
        Serial.println("Mqtt Reconnect failed!");
        return false;
    }
    return true;
}

void MqttClient::tick()
{
    if (!mqttClient.connected())
    {
        if (!reconnectMQTT(5))
            Serial.println("mqtt connection failed!");
    }
    else
    {
        mqttClient.publish("keepAlive/abc", "hello world");
        mqttClient.loop();
    }
}