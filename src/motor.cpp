#include "Arduino.h"
#include "motor.h"
#include "mqtt_message.h"


void IRAM_ATTR stop() 
{
    ledcWrite(MOTOR_L_PWM_CHAN, 0);
    ledcWrite(MOTOR_R_PWM_CHAN, 0);
}

Motor::Motor()
{
    pinMode(MOTOR_L_PWM_PIN, OUTPUT);
    ledcSetup(MOTOR_L_PWM_CHAN, MOTOR_PWM_FREQ_HZ, MOTOR_PWM_RES_BIT);
    ledcAttachPin(MOTOR_L_PWM_PIN, MOTOR_L_PWM_CHAN);
    ledcWrite(MOTOR_L_PWM_CHAN, 0); //off

    pinMode(MOTOR_R_PWM_PIN, OUTPUT);
    ledcSetup(MOTOR_R_PWM_CHAN, MOTOR_PWM_FREQ_HZ, MOTOR_PWM_RES_BIT);
    ledcAttachPin(MOTOR_R_PWM_PIN, MOTOR_R_PWM_CHAN);
    ledcWrite(MOTOR_R_PWM_CHAN, 0); //off

    pinMode(MOTOR_L_DIR_PIN, OUTPUT);
    digitalWrite(MOTOR_L_DIR_PIN, LOW);

    pinMode(MOTOR_R_DIR_PIN, OUTPUT);
    digitalWrite(MOTOR_R_DIR_PIN, LOW);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &stop, true);
  
}

void Motor::action(Message* message) {
    std::string payload = message->get_message();
    if(payload.rfind(ACTION_FORWARD, 0) == 0) {
        forward();
    } else if (payload.rfind(ACTION_LEFT, 0) == 0) {
        left();
    } else if (payload.rfind(ACTION_RIGHT, 0) == 0) {
        right();
    } else if (payload.rfind(ACTION_FLASH, 0) == 0) {
        flash();
    } else {
        Serial.println("Unknown Message ACTION!");
    }
}

void Motor::left()
{
    drive(speed_left, 0);
}

void Motor::right()
{
    drive(0, speed_right);
}

void Motor::forward()
{
    drive(speed_left, speed_right);
}

void Motor::backward()
{
    drive(-speed_left, -speed_right);
}

void Motor::flash()
{

}

void Motor::drive(int left, int right)
{
    ledcWrite(MOTOR_L_PWM_CHAN, abs(left) * 1023);
    ledcWrite(MOTOR_R_PWM_CHAN, abs(right) * 1023);
    // directions depends on whether speed is negative or positive
    left > 0 ? digitalWrite(MOTOR_L_DIR_PIN, HIGH) : digitalWrite(MOTOR_L_DIR_PIN, LOW);
    right > 0 ? digitalWrite(MOTOR_R_DIR_PIN, HIGH) : digitalWrite(MOTOR_R_DIR_PIN, LOW);
    timerAlarmWrite(timer, 1000000, true); //useconds
    timerAlarmEnable(timer);
}
