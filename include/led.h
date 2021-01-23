#ifndef LED_H
#define LED_H

#include "esp_camera.h"
#include "Arduino.h"

#define CAMERA_MODEL_AI_THINKER

// Select camera module used on the board
#define CAMERA_MODULE_OV2640

#include "camera_pins.h"
#include "camera.h"

// Status and illumination LED's
static int lampVal = 0; // Current Lamp value, range 0-100, Start off

static int lampChannel = 7;         // a free PWM channel (some channels used by camera)
                             // according to https://esp32.com/viewtopic.php?t=11379 those are channel 1 and 2
static const int pwmfreq = 50000;   // 50K pwm frequency
static const int pwmresolution = 9; // duty cycle bit range
// https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms
static const int pwmIntervals = 100; // The number of Steps between the output being on and off
static float lampR;                  // The R value in the PWM graph equation (calculated in setup)

static void setup_led()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LED_OFF);

    ledcSetup(lampChannel, pwmfreq, pwmresolution); // configure LED PWM channel
    ledcWrite(lampChannel, lampVal);                // set initial value
    ledcAttachPin(LAMP_PIN, lampChannel);           // attach the GPIO pin to the channel
    // Calculate the PWM scaling R factor:
    // https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms
    lampR = (pwmIntervals * log10(2)) / (log10(pow(2, pwmresolution)));
}

// Notification LED
static void flashLED(int flashtime)
{
      digitalWrite(LED_PIN, LED_ON);  // On at full power.
      delay(flashtime);               // delay
      digitalWrite(LED_PIN, LED_OFF); // turn Off
}

#endif