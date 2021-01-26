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
        int lamp_value = 100; // Current Lamp value, range 0-100
        float lampR; // The R value in the PWM graph equation (calculated in setup)

        hw_timer_t * motor_timer = NULL;
        hw_timer_t * flash_timer = NULL;
        void drive(int left, int right);
        

};


#endif