#ifndef _MAINAPP_
#define _MAINAPP_

#include "DHT.h"
#include "gui/main_menu.h"
#include "system/Pid.h"

#define RECEIVER_PIN 32

class MainApp {
    public:
        MainApp();
        ~MainApp();

        void run();
        
    private:
        int m_timer_sec;
        int m_timer_sec_old;
        float m_temperature;
        float m_humidity;

        bool m_2sec;

        /* Methods */
        void tick1sec();
        void closeAllMenu(BaseMenu * except);    
        void getThermoIgrometerData(); 
        void setHeater(bool status);
        
        /* Flags */
        bool m_single_machine_step;
      
        /* Plugins */
        DHT * m_dht;
        Pid * m_pid;

        /* Timers */
        UTimer m_1sec;        

        /* Callbacks */

        /* Menu */
        MainMenu * m_main_menu;
        
        /* Status Bar */

};


#endif