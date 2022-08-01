#ifndef _CONFIGDATA_
#define _CONFIGDATA_

#include <Arduino.h>

#define CONFIG_WIFI_SSID "wifissid"
#define CONFIG_WIFI_PASS "wifipass"
#define CONFIG_MQTT_HOST "mqtthost"
#define CONFIG_MQTT_USER "mqttuser"
#define CONFIG_MQTT_PASS "mqttpass"
#define CONFIG_SETPOINT "setpoint"
#define CONFIG_SETTIMER "settimer"

class ConfigData {
    public:
        ConfigData();
        ~ConfigData();

        void store();
        void store(const char * key, const char * val);
        void store(const char * key, const unsigned char val);
        void store(const char * key, int32_t val);

        void restore();
        String toString();

        String m_ssid;
        String m_pass;

        String m_mqtt_host;
        String m_mqtt_user;
        String m_mqtt_pass;

        int32_t m_setpoint;
        int32_t m_settimer;
};

extern ConfigData configData;

#endif