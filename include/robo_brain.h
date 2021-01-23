#ifndef ROBO_BRAIN_H
#define ROBO_BRAIN_H

#include "mqtt_client.h"
#include "motor.h"
#include "http_server.h"

class RoboBrain {
    public:
        RoboBrain();
        void tick();
    private:
        MqttClient mqtt;
        Motor motor;
        HttpServer http_server;

        void halt_and_blink();
};

#endif