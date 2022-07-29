#include "MainApp.h"
#include "system/MainStates.h"
#include "string.h"
#include "system/Rele.h"

/**
 * @brief Construct a new Main App:: Main App object
 * 
 */
MainApp::MainApp(){        
    main_state = MAIN_STATE::MAIN_MENU;   
    m_1sec.reset();
    m_single_machine_step = false;

    /* Menu */
    m_main_menu = NULL;  
    
    /* Status Bar */

    /* Plugins */
    m_dht = new DHT(32, DHT22);
    m_dht->begin();
    m_2sec = false;
    m_pid = NULL;

    
}

/**
 * @brief Destroy the Main App:: Main App object
 * 
 */
MainApp::~MainApp(){
    /* Menu */
    if(m_main_menu) delete m_main_menu;

    /* Status Bar */

    /* Plugins */
    if(m_dht) delete m_dht;
}

/**
 * @brief Main loop and state machine, to be executed as fast as possible
 * 
 */
void MainApp::run(){
    bool heater;
    float action;

    /* Esegue loop esterni */

    /* Macchina a stati principale */
    switch(main_state){
            case MAIN_STATE::MAIN_MENU:
                if(m_main_menu == NULL){
                    m_main_menu = new MainMenu();
                }
                break;
            case MAIN_STATE::START:
                if(m_pid) delete m_pid;
                m_pid = new Pid(m_main_menu->getTarget(),0.5);
                main_state = RAMPUP;
                break;
            case MAIN_STATE::RAMPUP:        
                heater = m_pid->ramp_up(m_temperature);
                //Serial.print("heater="); Serial.println(heater);
                RELE.setState(heater);
                if(!heater){
                    Serial.println("END RAMP UP");
                    main_state = PID;                
                }
                break;  
            case MAIN_STATE::PID:
                action = m_pid->PID_control(m_temperature);
                if(action > -1){
                   /*
                   if(action > 127) RELE.setState(true);
                   else RELE.setState(false);
                   */
                   RELE.setStatePWM(action,30);
                   Serial.printf("action=%f\n",action);
                }
                break;   
    }
    

    /* Tick 1 secondo */
    if(m_1sec.elapsed() > 1000000){
        tick1sec();        
        m_1sec.reset();
    }

}


/**
 * @brief Close all menu except one
 * 
 * @param except menu to mantain active or NULL to close all
 */
void MainApp::closeAllMenu(BaseMenu * except){
    if(except != m_main_menu && m_main_menu){delete m_main_menu; m_main_menu = NULL;}
}


/**
 * @brief Viene eseguito ogni secondo
 * 
 */
void MainApp::tick1sec(){
    if(m_2sec){
        getThermoIgrometerData();
        m_main_menu->setChartData(m_temperature,m_humidity);
        m_2sec = false;        
    }else{
        m_2sec = true;
    }
}

/**
 * @brief Legge i dati dal termoigrometro
 * 
 */
void MainApp::getThermoIgrometerData(){
    float t = m_dht->readTemperature();
    float h = m_dht->readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println("Errore lettura sensore");
        return;
    }

    m_temperature = t;
    m_humidity = h;

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.println(F("°C "));
}

void MainApp::setHeater(bool status){

}