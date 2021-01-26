#ifndef ROBO_COMS_H
#define ROBO_COMS_H

#include "mqtt_client.h"



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
        bool parse_action(Message** message);
};


#endif