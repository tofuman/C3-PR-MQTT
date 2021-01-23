#include <FS.h>
#include "SPIFFS.h"
#include "config.h"

// General KEYS
#define WIFI_SSID "wifi_ssid"
#define WIFI_PW "wifi_pw"
#define MQTT_BROKER "mqtt_broker"
#define MQTT_PORT "mqtt_port"

// Personal Keys
#define PERSONAL_NAME "name"

// Video Keys

Config::Config()
{
    wifi_ssid[0] = 0;
    wifi_pw[0] = 0;
    mqtt_broker[0] = 0;
    name[0] = 0;
}

bool Config::parse(const char *config_file)
{
    char buffer[64];

    File dataFile = SPIFFS.open(config_file);
    if (!dataFile || dataFile.isDirectory())
    {
        Serial.println("failed to open config");
        return false;
    }
    while (dataFile.available())
    {
        int l = dataFile.readBytesUntil('\n', buffer, sizeof(buffer));
        if (l >= 64)
        {
            dataFile.close();
            Serial.println("config line to long");
            return false;
        }
        buffer[l] = 0;
        if (buffer[0] == '#' || l <= 1)
        {
            continue;
        }
        bool ret = this->parse_line(buffer);
        if (!ret)
        {
            dataFile.close();
            return false;
        }
    }
    dataFile.close();
    return true;
}

bool Config::parse_line(char *line)
{
    char *key;
    char value[30];

    key = strtok(line, ":");
    if (!key)
    {
        Serial.println("no key in config line");
        return false;
    }
    strcpy(value, line+strlen(key)+1);
    if (strlen(value) == 0)
    {
        Serial.println("no value in config line");
        return false;
    }

    if (parse_key_general(key, value))
    {
        return true;
    }
    else if (parse_key_personal(key, value))
    {
        return true;
    }
    else if (parse_key_personal(key, value))
    {
        return true;
    }
    else
    {
        Serial.println("Unknown config key!");
        return false;
    }

    return true;
}

bool Config::parse_key_general(char *key, char *value)
{
    if (strcmp(key, WIFI_SSID) == 0)
    {
        strcpy(this->wifi_ssid, value);
    }
    else if (strcmp(key, WIFI_PW) == 0)
    {
        strcpy(this->wifi_pw, value);
    }
    else if (strcmp(key, MQTT_BROKER) == 0)
    {
        strcpy(this->mqtt_broker, value);
    }
    else if (strcmp(key, MQTT_PORT) == 0)
    {
        this->mqtt_port = atoi(value);
        if (this->mqtt_port == 0)
        {
            Serial.print(value);
            Serial.println(" => mqtt port in config wrong!");
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool Config::parse_key_personal(char *key, char *value)
{
    if (strcmp(key, PERSONAL_NAME) == 0)
    {
        strcpy(this->name, value);
    }
    else
    {
        return false;
    }
    return true;
}

bool Config::parse_key_video(char *key, char *value)
{

    return false;
}