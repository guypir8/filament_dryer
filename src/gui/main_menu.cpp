#include "main_menu.h"
#include <Arduino.h>
#include "system/ConfigData.h"
#include "config.h"


MainMenu* MainMenu::m_pMainMenu = 0;

MainMenu::MainMenu() : BaseMenu()
{

    m_pMainMenu = this;

    m_target = configData.m_setpoint;
    m_timer_h = configData.m_settimer/3600;
    m_timer_m = (configData.m_settimer - (m_timer_h*3600)) / 60;

    m_screen = api_create_screen();

    char txt[20];

    /**
     * @brief Create a Chart
     * 
     */
    m_chart = lv_chart_create(m_screen);
    lv_obj_set_size(m_chart, 185, 180);
    lv_obj_set_align(m_chart, LV_ALIGN_TOP_MID);
    lv_chart_set_type(m_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(m_chart,LV_CHART_UPDATE_MODE_SHIFT);
    lv_obj_set_style_size(m_chart, 0, LV_PART_INDICATOR);
    lv_obj_add_event_cb(m_chart, CHART_onEvent, LV_EVENT_DRAW_PART_BEGIN, NULL);

    lv_chart_set_range(m_chart,LV_CHART_AXIS_PRIMARY_Y,200,800);
    lv_chart_set_range(m_chart,LV_CHART_AXIS_SECONDARY_Y,0,1000);

    lv_chart_set_div_line_count(m_chart,10,10);

    lv_chart_set_axis_tick(m_chart, LV_CHART_AXIS_PRIMARY_Y, 2, 1, 7, 2, true, 50);
    lv_chart_set_axis_tick(m_chart, LV_CHART_AXIS_SECONDARY_Y, 2, 1, 6, 4, true, 50);

    lv_chart_set_point_count(m_chart, CHART_POINTS);


    /* Add 3 data series */
    m_chart_ser_temperature = lv_chart_add_series(m_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    m_chart_ser_humidity = lv_chart_add_series(m_chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_SECONDARY_Y);
    m_chart_ser_target = lv_chart_add_series(m_chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_all_value(m_chart,m_chart_ser_target,m_target*10);

    /**
     * @brief Create labels for actual data
     * 
     */
    m_lbl_temperature = lv_label_create(m_screen);
    lv_label_set_long_mode(m_lbl_temperature, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_temperature, "---.- °C");
    lv_obj_set_style_text_align(m_lbl_temperature, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_pos(m_lbl_temperature,150,10);
    lv_obj_set_style_text_color(m_lbl_temperature,lv_color_hex(0xff0000),0);

    m_lbl_humidity = lv_label_create(m_screen);
    lv_label_set_long_mode(m_lbl_humidity, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_humidity, "---.- %");
    lv_obj_set_style_text_align(m_lbl_humidity, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_pos(m_lbl_humidity,150,25);
    lv_obj_set_style_text_color(m_lbl_humidity,lv_color_hex(0x0000ff),0);

    LV_IMG_DECLARE(fire);
    m_fire = lv_img_create(m_screen);
    lv_img_set_src(m_fire, &fire);
    lv_obj_align(m_fire, LV_ALIGN_TOP_LEFT, 30, 5);
    lv_obj_set_size(m_fire, 48, 48);
    lv_obj_add_flag(m_fire,LV_OBJ_FLAG_HIDDEN);


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                  MENU DI SELEZIONE
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    m_canvas_main = lv_canvas_create(m_screen);
    lv_obj_set_pos(m_canvas_main,0,190);
    lv_obj_set_size(m_canvas_main,240,130);

    /**
     * @brief Create target temperature spinbox
     * 
     */    
    sprintf(txt,"%2d °C",m_target);

    m_lbl_target = lv_label_create(m_canvas_main);
    lv_label_set_long_mode(m_lbl_target, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_target, txt);
    lv_obj_set_style_text_align(m_lbl_target, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_target, 100);
    lv_obj_align(m_lbl_target,LV_ALIGN_TOP_MID,0,0);
    lv_obj_set_style_text_font(m_lbl_target,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_target,lv_color_make(0,255,0),0);
    
    lv_obj_t * btn = lv_btn_create(m_canvas_main);
    lv_obj_set_size(btn, 40, 30);
    lv_obj_align_to(btn, m_lbl_target, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, BTN_temp_inc, LV_EVENT_CLICKED,  NULL);
    lv_obj_add_event_cb(btn, BTN_temp_inc, LV_EVENT_LONG_PRESSED_REPEAT,  NULL);

    btn = lv_btn_create(m_canvas_main);
    lv_obj_set_size(btn, 40, 30);
    lv_obj_align_to(btn, m_lbl_target, LV_ALIGN_OUT_LEFT_MID, -10, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, BTN_temp_dec, LV_EVENT_CLICKED, NULL);    
    lv_obj_add_event_cb(btn, BTN_temp_dec, LV_EVENT_LONG_PRESSED_REPEAT, NULL); 

    /**
     * @brief Change timer
     * 
     */
    sprintf(txt,"%02d",m_timer_h);
    m_lbl_timer_h = lv_label_create(m_canvas_main);
    lv_obj_set_style_text_align(m_lbl_timer_h, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(m_lbl_timer_h, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_timer_h, txt);
    lv_obj_set_style_text_align(m_lbl_timer_h, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_timer_h, 50);
    lv_obj_align_to(m_lbl_timer_h,m_lbl_target,LV_ALIGN_OUT_BOTTOM_MID,-25,10);
    lv_obj_set_style_text_font(m_lbl_timer_h,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_timer_h,lv_color_make(0,255,0),0);
    
    sprintf(txt,"%02d",m_timer_m);
    m_lbl_timer_m = lv_label_create(m_canvas_main);
    lv_obj_set_style_text_align(m_lbl_timer_m, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(m_lbl_timer_m, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_timer_m, txt);
    lv_obj_set_style_text_align(m_lbl_timer_m, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_timer_m, 50);
    lv_obj_align_to(m_lbl_timer_m,m_lbl_target,LV_ALIGN_OUT_BOTTOM_MID,+25,10);
    lv_obj_set_style_text_font(m_lbl_timer_m,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_timer_m,lv_color_make(0,255,0),0);    
    
    lv_obj_t * lbl = lv_label_create(m_canvas_main);
    lv_label_set_text(lbl, ":");
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(lbl, 10);
    lv_obj_align_to(lbl,m_lbl_target,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    lv_obj_set_style_text_font(lbl,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(lbl,lv_color_make(0,255,0),0);    

    btn = lv_btn_create(m_canvas_main);
    lv_obj_set_size(btn, 40, 30);
    lv_obj_align_to(btn, m_lbl_timer_m, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, BTN_timer_m_inc, LV_EVENT_CLICKED,  NULL);
    lv_obj_add_event_cb(btn, BTN_timer_h_inc, LV_EVENT_LONG_PRESSED_REPEAT,  NULL);

    btn = lv_btn_create(m_canvas_main);
    lv_obj_set_size(btn, 40, 30);
    lv_obj_align_to(btn, m_lbl_timer_h, LV_ALIGN_OUT_LEFT_MID, -10, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, BTN_timer_m_dec, LV_EVENT_CLICKED, NULL);    
    lv_obj_add_event_cb(btn, BTN_timer_h_dec, LV_EVENT_LONG_PRESSED_REPEAT, NULL); 


    /**
     * @brief Create START button
     * 
     */
    btn = lv_btn_create(m_canvas_main);
    //lv_obj_add_event_cb(btn, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_width(btn,200);
    lbl = lv_label_create(btn);    
    lv_label_set_text(lbl, "START");
    lv_obj_center(lbl);
    lv_obj_set_style_text_font(lbl,&lv_font_montserrat_28,0);    
    lv_obj_set_style_bg_color(btn,lv_color_make(0,200,0),0);
    lv_obj_set_style_radius(btn,20,0);    
    lv_obj_add_event_cb(btn, BTN_start_click, LV_EVENT_CLICKED, NULL);    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                  MENU DI ESECUZIONE
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
    m_canvas_exec = lv_canvas_create(m_screen);
    lv_obj_set_pos(m_canvas_exec,0,190);
    lv_obj_set_size(m_canvas_exec,240,130);

    lv_obj_add_flag(m_canvas_exec,LV_OBJ_FLAG_HIDDEN);

    m_lbl_run_temperature = lv_label_create(m_canvas_exec);
    lv_label_set_long_mode(m_lbl_run_temperature, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_run_temperature, txt);
    lv_obj_set_style_text_align(m_lbl_run_temperature, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_run_temperature, 120);
    lv_obj_align(m_lbl_run_temperature,LV_ALIGN_TOP_LEFT,0,0);
    lv_obj_set_style_text_font(m_lbl_run_temperature,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_run_temperature,lv_color_make(255,0,0),0);

    m_lbl_run_target = lv_label_create(m_canvas_exec);
    lv_label_set_long_mode(m_lbl_run_target, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_run_target, txt);
    lv_obj_set_style_text_align(m_lbl_run_target, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_run_target, 120);
    lv_obj_align(m_lbl_run_target,LV_ALIGN_TOP_RIGHT,0,0);
    lv_obj_set_style_text_font(m_lbl_run_target,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_run_target,lv_color_make(0,255,0),0);

    LV_FONT_DECLARE(digitmono_36);
    m_lbl_run_timer = lv_label_create(m_canvas_exec);
    lv_label_set_long_mode(m_lbl_run_timer, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_run_timer, "...");
    lv_obj_set_style_text_align(m_lbl_run_timer, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_run_timer, 240);
    lv_obj_align(m_lbl_run_timer,LV_ALIGN_TOP_MID,0,36);
    lv_obj_set_style_text_font(m_lbl_run_timer,&digitmono_36,0);
    lv_obj_set_style_text_color(m_lbl_run_timer,lv_color_make(255,255,255),0);

    btn = lv_btn_create(m_canvas_exec);
    //lv_obj_add_event_cb(btn, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_width(btn,200);
    lbl = lv_label_create(btn);    
    lv_label_set_text(lbl, "STOP");
    lv_obj_center(lbl);
    lv_obj_set_style_text_font(lbl,&lv_font_montserrat_28,0);    
    lv_obj_set_style_bg_color(btn,lv_color_make(255,0,0),0);
    lv_obj_set_style_radius(btn,20,0);    
    lv_obj_add_event_cb(btn, BTN_stop_click, LV_EVENT_CLICKED, NULL);    


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                  MENU DI ALLARME
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
    
    m_canvas_alarm = lv_canvas_create(m_screen);
    lv_obj_set_pos(m_canvas_alarm,0,190);
    lv_obj_set_size(m_canvas_alarm,240,130);

    lv_obj_add_flag(m_canvas_alarm,LV_OBJ_FLAG_HIDDEN);

    
    m_lbl_alarm_desc = lv_label_create(m_canvas_alarm);
    lv_label_set_long_mode(m_lbl_alarm_desc, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_alarm_desc, "...");
    lv_obj_set_style_text_align(m_lbl_alarm_desc, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_alarm_desc, 240);
    lv_obj_align(m_lbl_alarm_desc,LV_ALIGN_TOP_MID,0,10);
    lv_obj_set_style_text_font(m_lbl_alarm_desc,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_alarm_desc,lv_color_make(255,255,255),0);


    btn = lv_btn_create(m_canvas_alarm);
    //lv_obj_add_event_cb(btn, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_width(btn,200);
    lbl = lv_label_create(btn);    
    lv_label_set_text(lbl, "RESET");
    lv_obj_center(lbl);
    lv_obj_set_style_text_font(lbl,&lv_font_montserrat_28,0);    
    lv_obj_set_style_bg_color(btn,lv_color_make(255,0,0),0);
    lv_obj_set_style_radius(btn,20,0);    
    lv_obj_add_event_cb(btn, BTN_alarm_click, LV_EVENT_CLICKED, NULL);    
    

    lv_scr_load(m_screen);
}

MainMenu::~MainMenu()
{
    lv_obj_del(m_screen);
    m_screen = NULL;
}

/**
 * @brief 
 * 
 * @param temperature 
 * @param humidity 
 */
void MainMenu::setChartData(float temperature, float humidity)
{

    if (humidity == -1 || temperature == -1) {
        return;
    }

    char txt[20];
    lv_chart_set_next_value(m_chart, m_chart_ser_temperature, temperature*10);
    lv_chart_set_next_value(m_chart, m_chart_ser_humidity, humidity*10);

    sprintf(txt,"% 3.1f °C",temperature);
    lv_label_set_text(m_lbl_temperature, txt);
    sprintf(txt,"% 3.1f %%",humidity);
    lv_label_set_text(m_lbl_humidity, txt);

    sprintf(txt,"% 3.1f °C",temperature);
    lv_label_set_text(m_lbl_run_temperature, txt);

    sprintf(txt,"% 3.1d °C",m_target);
    lv_label_set_text(m_lbl_run_target, txt);
    
}

/**
 * @brief 
 * 
 * @param e 
 */
void MainMenu::CHART_onEvent(lv_event_t *e)
{

    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_DRAW_PART_BEGIN){
        lv_obj_draw_part_dsc_t* dsc_chart = lv_event_get_draw_part_dsc(e);
        if (dsc_chart->part = LV_PART_TICKS) {
            if (dsc_chart->id == LV_CHART_AXIS_PRIMARY_Y) {            
                if(dsc_chart->text){
                    dsc_chart->label_dsc->color = lv_color_make(255, 0, 0);
                    float f = String(dsc_chart->text).toFloat();
                    strcpy(dsc_chart->text,String((int)(f/10)).c_str());
                }
            }
            else if (dsc_chart->id == LV_CHART_AXIS_SECONDARY_Y) {
                if(dsc_chart->text){
                    dsc_chart->label_dsc->color = lv_color_make(0, 0, 255);
                    float f = String(dsc_chart->text).toFloat();
                    strcpy(dsc_chart->text,String((int)(f/10)).c_str());                    
                }
            }               
        }
    }
}

void MainMenu::BTN_temp_inc(lv_event_t *e){
    char txt[20];    
    m_pMainMenu->m_target++;
    if( (m_pMainMenu->m_target) > 80) m_pMainMenu->m_target = 80;
    sprintf(txt,"%2d °C",m_pMainMenu->m_target);
    lv_label_set_text(m_pMainMenu->m_lbl_target,txt);
    lv_chart_set_all_value(m_pMainMenu->m_chart,m_pMainMenu->m_chart_ser_target,m_pMainMenu->m_target*10);
}

void MainMenu::BTN_temp_dec(lv_event_t *e){
    char txt[20];    
    m_pMainMenu->m_target--;
    if( (m_pMainMenu->m_target) < 20) m_pMainMenu->m_target = 20;
    sprintf(txt,"%2d °C",m_pMainMenu->m_target);
    lv_label_set_text(m_pMainMenu->m_lbl_target,txt);
    lv_chart_set_all_value(m_pMainMenu->m_chart,m_pMainMenu->m_chart_ser_target,m_pMainMenu->m_target*10);
}

void MainMenu::BTN_timer_h_inc(lv_event_t *e){
    char txt[20];    
    if( m_pMainMenu->m_long_press_timeout.elapsed() < 1000000 ) return;
    m_pMainMenu->m_timer_h++;
    if( (m_pMainMenu->m_timer_h) > 6){
        m_pMainMenu->m_timer_h = 6;
    }
    sprintf(txt,"%02d",m_pMainMenu->m_timer_h);
    lv_label_set_text(m_pMainMenu->m_lbl_timer_h,txt);  
    m_pMainMenu->m_long_press_timeout.reset();
}

void MainMenu::BTN_timer_h_dec(lv_event_t *e){
    char txt[20];    
    if( m_pMainMenu->m_long_press_timeout.elapsed() < 1000000 ) return;
    m_pMainMenu->m_timer_h--;
    if( (m_pMainMenu->m_timer_h) < 0){
        m_pMainMenu->m_timer_h = 0;
    }
    sprintf(txt,"%02d",m_pMainMenu->m_timer_h);
    lv_label_set_text(m_pMainMenu->m_lbl_timer_h,txt);  
    m_pMainMenu->m_long_press_timeout.reset();
}

void MainMenu::BTN_timer_m_inc(lv_event_t *e){
    char txt[20];    
    m_pMainMenu->m_timer_m++;
    if( (m_pMainMenu->m_timer_m) > 59){
        m_pMainMenu->m_timer_m = 0;
    }
    sprintf(txt,"%02d",m_pMainMenu->m_timer_m);
    lv_label_set_text(m_pMainMenu->m_lbl_timer_m,txt);
}

void MainMenu::BTN_timer_m_dec(lv_event_t *e){
    char txt[20];    
    m_pMainMenu->m_timer_m--;
    if( (m_pMainMenu->m_timer_m) < 0){
        m_pMainMenu->m_timer_m = 59;
    }
    sprintf(txt,"%02d",m_pMainMenu->m_timer_m);
    lv_label_set_text(m_pMainMenu->m_lbl_timer_m,txt);
}

void MainMenu::BTN_start_click(lv_event_t *e){
    lv_obj_add_flag(m_pMainMenu->m_canvas_main,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(m_pMainMenu->m_canvas_exec,LV_OBJ_FLAG_HIDDEN);
    main_state = START;
}

void MainMenu::BTN_stop_click(lv_event_t *e){
    lv_obj_add_flag(m_pMainMenu->m_canvas_exec,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(m_pMainMenu->m_canvas_main,LV_OBJ_FLAG_HIDDEN);
    main_state = MAIN_MENU;
}

void MainMenu::BTN_alarm_click(lv_event_t *e){
    lv_obj_add_flag(m_pMainMenu->m_canvas_alarm,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(m_pMainMenu->m_canvas_main,LV_OBJ_FLAG_HIDDEN);
    main_state = MAIN_MENU;
}

float MainMenu::getTarget(){
    return m_target;
}

int MainMenu::getTimerSec(){
    return (m_timer_h * 3600) + (m_timer_m * 60);
}

void MainMenu::setTimerString(char * txt){
    lv_label_set_text(m_pMainMenu->m_lbl_run_timer,txt);  
}

void MainMenu::stop(){
    BTN_stop_click(NULL);
}

void MainMenu::alarm(String desc){
    lv_label_set_text(m_lbl_alarm_desc,desc.c_str());  
    lv_obj_add_flag(m_pMainMenu->m_canvas_exec,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(m_pMainMenu->m_canvas_main,LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(m_pMainMenu->m_canvas_alarm,LV_OBJ_FLAG_HIDDEN);
}

void MainMenu::setHeater(bool heater){
    if(heater){
        lv_obj_clear_flag(m_fire,LV_OBJ_FLAG_HIDDEN);        
    }else{
        lv_obj_add_flag(m_fire,LV_OBJ_FLAG_HIDDEN);        
    }
}