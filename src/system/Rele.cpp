#include <Arduino.h>
#include "Rele.h"

Rele::Rele(){
    m_state = false;
    pinMode(PIN_RELE, OUTPUT);
    digitalWrite(PIN_RELE,m_state?HIGH:LOW);
    m_pwm_started = false;
}

void Rele::setState(bool state){
    m_state = state;
    digitalWrite(PIN_RELE,m_state?HIGH:LOW);
}

bool Rele::getState(){
    return m_state;
}

void Rele::setStatePWM(int pwm255, int period_sec){
    if(!m_pwm_started){        
        m_pwm_time_on = (period_sec * pwm255)/255;
        if(m_pwm_time_on < 5) return;
        
        m_pwm_time_period = period_sec;
        m_pwm_timer.reset();
        setState(true);
        m_pwm_started = true;
    }else{
        if( m_pwm_timer.elapsed() >= (m_pwm_time_period*1000000) ){
            m_pwm_started = false;
        }else if(m_pwm_timer.elapsed() >= (m_pwm_time_on*1000000) ){
            setState(false);
        }
    }
}

Rele RELE;