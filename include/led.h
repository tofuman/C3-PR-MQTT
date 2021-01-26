#ifndef LED_H
#define LED_H

#include "esp_camera.h"
#include "Arduino.h"

#define CAMERA_MODEL_AI_THINKER

// Select camera module used on the board
#define CAMERA_MODULE_OV2640

#include "camera_pins.h"
#include "camera.h"


static void setup_led()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LED_OFF);
}

// Notification LED
static void flashLED(int flashtime)
{
      digitalWrite(LED_PIN, LED_ON);  // On at full power.
      delay(flashtime);               // delay
      digitalWrite(LED_PIN, LED_OFF); // turn Off
}

#endif