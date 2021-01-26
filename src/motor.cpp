#include <mutex>

// we need this include for the lamp pins, which are a feature of the camera module.
#define CAMERA_MODEL_AI_THINKER
#define CAMERA_MODULE_OV2640
#include "camera_pins.h"

#include "Arduino.h"
#include "motor.h"
#include "mqtt_message.h"

static const int lampChannel = 7; // a free PWM channel (some channels used by camera)
// according to https://esp32.com/viewtopic.php?t=11379 those are channel 1 and 2
static const int pwmfreq = 50000;   // 50K pwm frequency
static const int pwmresolution = 9; // duty cycle bit range
// https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms
static const int pwmIntervals = 100; // The number of Steps between the output being on and off

static std::mutex moving_lock;
static std::mutex flash_lock;

void IRAM_ATTR motor_stop()
{
    ledcWrite(MOTOR_L_PWM_CHAN, 0);
    ledcWrite(MOTOR_R_PWM_CHAN, 0);
    moving_lock.unlock();
}

void IRAM_ATTR flash_stop()
{
    ledcWrite(lampChannel, 0);
    flash_lock.unlock();
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

    motor_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(motor_timer, &motor_stop, true);

    flash_timer = timerBegin(1, 80, true);
    timerAttachInterrupt(flash_timer, &flash_stop, true);

    ledcSetup(lampChannel, pwmfreq, pwmresolution); // configure LED PWM channel
    ledcWrite(lampChannel, 0);                      // set initial value
    ledcAttachPin(LAMP_PIN, lampChannel);           // attach the GPIO pin to the channel
    // Calculate the PWM scaling R factor:
    // https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms
    lampR = (pwmIntervals * log10(2)) / (log10(pow(2, pwmresolution)));
}

void Motor::action(Message *message)
{
    switch (message->get_action())
    {
    case ACTION_FORWARD:
        forward();
        break;
    case ACTION_LEFT:
        left();
        break;
    case ACTION_RIGHT:
        right();
        break;
    case ACTION_BACK:
        backward();
        break;
    case ACTION_FLASH:
        flash();
        break;
    default:
        Serial.println("Got Bogus Action!");
        break;
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
    flash_lock.lock();
    if (lamp_value > 100)
        lamp_value = 100; // normalise 0-255 (pwm range) just in case..
    if (lamp_value < 0)
        lamp_value = 0;
    // https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms
    int brightness = pow(2, (lamp_value / lampR)) - 1;
    ledcWrite(lampChannel, brightness);
    timerAlarmWrite(motor_timer, 1000000, true); //useconds
    timerAlarmEnable(motor_timer);
}

void Motor::drive(int left, int right)
{
    moving_lock.lock();
    ledcWrite(MOTOR_L_PWM_CHAN, abs(left) * 1023);
    ledcWrite(MOTOR_R_PWM_CHAN, abs(right) * 1023);
    // directions depends on whether speed is negative or positive
    left > 0 ? digitalWrite(MOTOR_L_DIR_PIN, HIGH) : digitalWrite(MOTOR_L_DIR_PIN, LOW);
    right > 0 ? digitalWrite(MOTOR_R_DIR_PIN, HIGH) : digitalWrite(MOTOR_R_DIR_PIN, LOW);
    timerAlarmWrite(motor_timer, 1000000, true); //useconds
    timerAlarmEnable(motor_timer);
}
