#include <esp_wifi.h>
#include <WiFi.h>
#include "SPIFFS.h"

#include "pins.h"
#include "mqtt_client.h"
#include "robo_brain.h"
#include "config.h"
#include "led.h"

#define GENERAL_CONFIG_FILE "/general_config"
#define VIDEO_CONFIG_FILE "/video_config"
#define PERSONAL_CONFIG_FILE "/personal_config"

// This will be displayed to identify the firmware
char myVer[] PROGMEM = __DATE__ " @ " __TIME__;

RoboBrain::RoboBrain() :motor(), coms(), http_server()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.print("Code Built: ");
    Serial.println(myVer);
    //init Filessytem for /data dir
    SPIFFS.begin(true);

    setup_led();

    config = Config();
    if (!config.parse(GENERAL_CONFIG_FILE))
        halt_and_blink();
    if (!config.parse(VIDEO_CONFIG_FILE))
        halt_and_blink();
    if (!config.parse(PERSONAL_CONFIG_FILE))
        halt_and_blink();

    

    // Feedback that hardware init is complete and we are now attempting to connect
    Serial.println("");
    Serial.print("Connecting to Wifi network: ");
    Serial.println(config.get_wifi_ssid());

    WiFi.begin(config.get_wifi_ssid(), config.get_wifi_pw());

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(250); // Wait for Wifi to connect. If this fails wifi the code basically hangs here.
                    // - It would be good to do something else here as a future enhancement.
                    //   (eg: go to a captive AP config portal to configure the wifi)
    }

    // feedback that we are connected
    Serial.print("WiFi connected IP:");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
    
    if (!coms.setup(&config))
        halt_and_blink();
    Serial.println("MQTT Ready");

    if (!http_server.setup())
        halt_and_blink();
    Serial.print("Camera Ready!  Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
}

void RoboBrain::tick()
{
    if(WiFi.status() != WL_CONNECTED){
        WiFi.reconnect();
    }
    coms.tick();
    Message* new_message = coms.fetch_message();
    if (new_message) {
        if (new_message->get_type() == MESSAGE_ACTION) {
            motor.action(new_message);
        }
        delete new_message;
    }
    flashLED(200); //blink led as heartbeat
    // Just loop forever.
    // The stream and URI handler processes initiated by the startCameraServer() call at the
    // end of setup() will handle the camera and UI processing from now on.
    delay(5000);
}

void RoboBrain::halt_and_blink()
{
    Serial.println("HALT!!!!");
    for (int i = 0; i < 600; i++)
    {
        flashLED(500);
        delay(500);
    }
    ESP.restart();
}

