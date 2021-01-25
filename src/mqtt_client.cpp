
#include <string>
#include <vector>
#include <mutex>

#include "config.h"
#include "mqtt_client.h"

#define MAX_QUEUED_MESSAGES 20

static std::mutex message_mutex;
static std::vector<Message*> messages;

// MQTT client
static void callback(char *topic, byte *payload, unsigned int length)
{
    Message* new_message = new Message(topic, (char*)payload, length);
    Serial.print("Message arrived for: ");
    Serial.print(topic);
    Serial.println("");
    message_mutex.lock();
    if (messages.size() < MAX_QUEUED_MESSAGES){
        messages.push_back(new_message);
    } else {
        Serial.println("To many messages in Buffer! dropping!");
        delete new_message;
    }
    message_mutex.unlock();
   
}

bool MqttClient::setup(Config *config)
{
    this->config = config;
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
    Serial.println("Reconnecting to MQTT Broker..");
    while (!mqttClient.connected() && trys < retry)
    {

        if (mqttClient.connect(config->get_name()))
        {
            Serial.println("MQTT Connected.");
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
        mqttClient.loop();
    }
}

bool MqttClient::subscribe(char* topic)
{
    if (!mqttClient.connected())
    {
        if (!reconnectMQTT(5)){
            Serial.println("mqtt connection failed!");
            return false;
        }
    }
    return mqttClient.subscribe(topic);
}

bool MqttClient::publish(char* topic, char* message)
{
    if (!mqttClient.connected())
    {
        if (!reconnectMQTT(5))
        {
            Serial.println("mqtt connection failed!");
            return false;
        }            

    }
    return mqttClient.publish(topic, message);
}

int MqttClient::get_messages_pending() {
    int pending = 0;
    message_mutex.lock();
    pending = messages.size();
    message_mutex.unlock();
    return pending;
}

Message* MqttClient::get_message() 
{
    Message *pending_message = NULL;
    message_mutex.lock();
    if (!messages.empty()) {
        pending_message = messages.back();
        messages.pop_back();
    }
    message_mutex.unlock();
    return pending_message;
}
