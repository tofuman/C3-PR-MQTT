#include "Arduino.h"
#include "motor.h"

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
}