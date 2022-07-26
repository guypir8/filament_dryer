#ifndef _MAIN_MENU_
#define _MAIN_MENU_
#include "base_menu.h"

class MainMenu : public BaseMenu {
    public:
        MainMenu();
        ~MainMenu();

        void setChartData(float temperature, float humidity);

    private:
        static MainMenu * m_pMainMenu;

        lv_obj_t * m_chart;
        lv_chart_series_t * m_chart_ser_temperature;
        lv_chart_series_t * m_chart_ser_humidity;
        lv_chart_series_t * m_chart_ser_target;

        lv_obj_t * m_lbl_temperature;
        lv_obj_t * m_lbl_humidity;

        lv_obj_t * m_lbl_target;
        lv_obj_t * m_lbl_timer_h;
        lv_obj_t * m_lbl_timer_m;

        int m_target;
        int m_timer_h;
        int m_timer_m;
        
        static void CHART_onEvent(lv_event_t *e);
        static void BTN_temp_inc(lv_event_t *e);
        static void BTN_temp_dec(lv_event_t *e);
        static void BTN_timer_h_inc(lv_event_t *e);
        static void BTN_timer_h_dec(lv_event_t *e);        
        static void BTN_timer_m_inc(lv_event_t *e);
        static void BTN_timer_m_dec(lv_event_t *e);        

};


#endif