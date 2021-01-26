#ifndef MOTOR_H
#define MOTOR_H

#include "pins.h"

class Message;

class Motor {
    public:
        Motor();
        void action(Message* message);
        void left();
        void right();
        void forward();
        void backward();
        void flash();
    private:
        int speed_left = 100;
        int speed_right = 100;
        hw_timer_t * timer = NULL;
        void drive(int left, int right);
        

};


#endif