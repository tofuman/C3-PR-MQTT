#ifndef ROBO_COMS_H
#define ROBO_COMS_H

#include "mqtt_client.h"

#define TOPIC_RECV_ACTION "action/"
#define TOPIC_RECV_SETTING "setting/"
#define TOPIC_RECV_ALL "all"

#define TOPIC_SEND_STATUS "status/"
#define TOPIC_SEND_REGISTER "register"

class RoboComs {
    public:
        RoboComs(): mqtt() {};
        bool setup(Config* config);
        Message* fetch_message();
        void tick();

    private:
        MqttClient mqtt;
        Config *config;

        void parse_type(Message** message);
};


#endif