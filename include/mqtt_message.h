#ifndef MQTT_MESSAGE_H
#define MQTT_MESSAGE_H

#include <string>

enum MessageType {
    MESSAGE_NONE,
    MESSAGE_ACTION,
    MESSAGE_SETTING,
    MESSAGE_ALL
};

enum MessageAction{
    ACTION_NONE,
    ACTION_FORWARD,
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_BACK,
    ACTION_FLASH
};

enum MessageSetting {
    SETTING_NONE
};

enum MessageAll {
    ALL_NONE,
    ALL_HALT,
    ALL_REBOOT
};

union MessageContent {
    MessageAction action;
    MessageSetting setting;
    MessageAll all;
};


class Message
{
    public:
        Message(char *topic, char *payload, unsigned int length): 
                    topic(topic), 
                    message(payload,length),
                    is_empty(false){};
        Message(){};
        std::string get_message() {return message;};
        std::string get_topic() {return topic;};
        MessageType get_type() {return type;};
        void set_type(MessageType type) {this->type = type;};
        

    private:
        std::string topic;
        std::string message;
        bool is_empty = true;
        MessageType type = MESSAGE_NONE;
};

#endif