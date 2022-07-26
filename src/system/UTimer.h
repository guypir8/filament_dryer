#ifndef _UTIMER_
#define _UTIMER_
#include <Arduino.h>

class UTimer
{
    public:
        UTimer(){ reset(); }
        void reset(){ m_start = micros(); };
        unsigned long elapsed(){ return micros() - m_start; };
    
    private:
        unsigned long m_start;
};

#endif