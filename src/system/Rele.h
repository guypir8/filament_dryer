#ifndef _RELE_
#define _RELE_

#include "system/UTimer.h"
#include "config.h"

#define PIN_RELE RELE_PIN

class Rele {

    public:
        Rele();
        void setState(bool state);
        bool getState();
        void setStatePWM(int pwm255, int period_sec);

    private:
        bool m_state;
        int m_pwm_time_on;
        int m_pwm_time_period;
        bool m_pwm_started;
        UTimer m_pwm_timer;

};

extern Rele RELE;

#endif