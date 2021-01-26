#ifndef ROBO_COMS_H
#define ROBO_COMS_H

#include "mqtt_client.h"

static const std::string TOPIC_RECV_ACTION = std::string("action/");
static const std::string TOPIC_RECV_SETTING = std::string("setting/");
static const std::string  TOPIC_RECV_ALL = std::string("all");

static const std::string TOPIC_SEND_STATUS = std::string("status/");
static const std::string TOPIC_SEND_REGISTER = std::string("register");

class RoboComs {
    public:
        RoboComs(): mqtt() {};
        bool setup(Config* config);
        Message* fetch_message();
        void tick();

    private:
        MqttClient mqtt;
        Config *config;

        bool parse_type(Message** message);
};


#endif