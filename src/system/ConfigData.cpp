#include "configdata.h"
#include <Preferences.h>

ConfigData::ConfigData(){
    m_ssid = "SSID";
    m_pass = "PASS";
    m_mqtt_host = "mqtt.host";
    m_mqtt_user = "mqttuser";
    m_mqtt_pass = "mqttpass";
    m_setpoint = 50;
    m_settimer = 3600;
}

ConfigData::~ConfigData(){

}

void ConfigData::store(){
    Preferences config;
    config.begin("configdata",false);

    config.putString(CONFIG_WIFI_SSID,m_ssid);
    config.putString(CONFIG_WIFI_PASS,m_pass);    

    config.putString(CONFIG_MQTT_HOST,m_mqtt_host);
    config.putString(CONFIG_MQTT_USER,m_mqtt_user);
    config.putString(CONFIG_MQTT_PASS,m_mqtt_pass);

    config.putInt(CONFIG_SETPOINT,m_setpoint);
    config.putInt(CONFIG_SETTIMER,m_settimer);
    
    config.end();
}

void ConfigData::store(const char * key, int32_t val){
    Preferences config;
    config.begin("configdata",false);
    config.putInt(key,val);
    config.end();

    if( strcmp(key,CONFIG_SETPOINT)==0 ) m_setpoint = val;   
    if( strcmp(key,CONFIG_SETTIMER)==0 ) m_settimer = val;   
}

void ConfigData::store(const char * key, const char * val){
    Preferences config;
    config.begin("configdata",false);
    config.putString(key,val);
    config.end();

    if( strcmp(key,CONFIG_WIFI_SSID)==0 ) m_ssid = val;
    if( strcmp(key,CONFIG_WIFI_PASS)==0 ) m_pass = val;    

    if( strcmp(key,CONFIG_MQTT_HOST)==0 ) m_mqtt_host = val;    
    if( strcmp(key,CONFIG_MQTT_USER)==0 ) m_mqtt_user = val;    
    if( strcmp(key,CONFIG_MQTT_PASS)==0 ) m_mqtt_pass = val;        
}

void ConfigData::store(const char * key, const unsigned char val){
    Preferences config;
    config.begin("configdata",false);
    config.putUChar(key,val);
    config.end();
}

void ConfigData::restore(){
    Preferences config;
    config.begin("configdata",false);
    
    m_ssid = config.getString(CONFIG_WIFI_SSID,m_ssid);
    m_pass = config.getString(CONFIG_WIFI_PASS,m_pass);

    m_mqtt_host = config.getString(CONFIG_MQTT_HOST,m_mqtt_host);
    m_mqtt_user = config.getString(CONFIG_MQTT_USER,m_mqtt_user);
    m_mqtt_pass = config.getString(CONFIG_MQTT_PASS,m_mqtt_pass);

    m_setpoint = config.getInt(CONFIG_SETPOINT,m_setpoint);
    m_settimer = config.getInt(CONFIG_SETTIMER,m_settimer);

    config.end();
}

String ConfigData::toString(){
    String ret = "";
    ret += "m_ssid = " + m_ssid + "\n";
    ret += "m_pass = " + m_pass + "\n";
    ret += "m_mqtt_host = " + m_mqtt_host + "\n";
    ret += "m_mqtt_user = " + m_mqtt_user + "\n";
    ret += "m_mqtt_pass = " + m_mqtt_pass + "\n";
    ret += "m_setpoint = " + String(m_setpoint) + "\n";
    ret += "m_settimer = " + String(m_settimer) + "\n";

    return ret;
}

ConfigData configData;