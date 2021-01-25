#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>

class Config {
    public:
        Config();
        bool parse(const char *config_file);
        //general
        char* get_wifi_ssid() { return wifi_ssid;};
        char* get_wifi_pw() {return wifi_pw;};
        char* get_mqtt_broker_ip() { return mqtt_broker;};
        int get_mqtt_broker_port() { return mqtt_port;};
        //personal
        char* get_name() { return name;};
    private:
        //general
        char wifi_ssid[30];
        char wifi_pw[30];
        char mqtt_broker[30];
        int mqtt_port = 1883;

        //peronal
        char name[30];

        bool parse_line(char* line);
        bool parse_key_general(char* key, char* value);
        bool parse_key_personal(char* key, char* value);
        bool parse_key_video(char* key, char* value);

};

#endif