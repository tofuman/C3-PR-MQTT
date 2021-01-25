#ifndef ROBO_BRAIN_H
#define ROBO_BRAIN_H

#include "robo_coms.h"
#include "motor.h"
#include "http_server.h"
#include "config.h"



class RoboBrain {
    public:
        RoboBrain();
        void tick();
    private:
        Motor motor;
        RoboComs coms;
        HttpServer http_server;
        Config config;

        void halt_and_blink();
};

#endif