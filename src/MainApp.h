#ifndef _MAINAPP_
#define _MAINAPP_

#include "DHT.h"
#include "gui/main_menu.h"


#define RECEIVER_PIN 32

class MainApp {
    public:
        MainApp();
        ~MainApp();

        void run();
        
    private:

        float m_temperature;
        float m_humidity;

        bool m_2sec;

        /* Methods */
        void tick1sec();
        void closeAllMenu(BaseMenu * except);    
        void getThermoIgrometerData();    
        
        /* Flags */
        bool m_single_machine_step;
      
        /* Plugins */
        DHT * m_dht;

        /* Timers */
        UTimer m_1sec;        

        /* Callbacks */

        /* Menu */
        MainMenu * m_main_menu;
        
        /* Status Bar */

};


#endif