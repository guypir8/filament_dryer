#ifndef _MAIN_MENU_
#define _MAIN_MENU_
#include "base_menu.h"
#include "system/UTimer.h"

class MainMenu : public BaseMenu {
    public:
        MainMenu();
        ~MainMenu();

        void setChartData(float temperature, float humidity);

        float getTarget();
        int getTimerSec();
        void setTimerString(char * txt);
        void stop();
        void alarm(String desc);
        void setHeater(bool heater);
        
    private:

        static MainMenu * m_pMainMenu;

        UTimer m_long_press_timeout;

        lv_obj_t * m_chart;
        lv_chart_series_t * m_chart_ser_temperature;
        lv_chart_series_t * m_chart_ser_humidity;
        lv_chart_series_t * m_chart_ser_target;

        lv_obj_t * m_lbl_temperature;
        lv_obj_t * m_lbl_humidity;

        lv_obj_t * m_lbl_target;
        lv_obj_t * m_lbl_timer_h;
        lv_obj_t * m_lbl_timer_m;

        lv_obj_t * m_lbl_run_temperature;
        lv_obj_t * m_lbl_run_target;
        lv_obj_t * m_lbl_run_timer;

        lv_obj_t * m_lbl_alarm_desc;

        lv_obj_t * m_canvas_main;
        lv_obj_t * m_canvas_exec;
        lv_obj_t * m_canvas_alarm;

        lv_obj_t * m_fire;

        int m_target;
        int m_timer_h;
        int m_timer_m;
        
        static void CHART_onEvent(lv_event_t *e);
        static void BTN_start_click(lv_event_t *e);
        static void BTN_stop_click(lv_event_t *e);
        static void BTN_alarm_click(lv_event_t *e);
        static void BTN_temp_inc(lv_event_t *e);
        static void BTN_temp_dec(lv_event_t *e);
        static void BTN_timer_h_inc(lv_event_t *e);
        static void BTN_timer_h_dec(lv_event_t *e);        
        static void BTN_timer_m_inc(lv_event_t *e);
        static void BTN_timer_m_dec(lv_event_t *e);        

};


#endif