#include "main_menu.h"
#include <Arduino.h>

MainMenu* MainMenu::m_pMainMenu = 0;

MainMenu::MainMenu() : BaseMenu()
{

    m_pMainMenu = this;

    m_target = 20;
    m_timer_h = 0;
    m_timer_m = 30;

    m_screen = api_create_screen();

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

    lv_chart_set_range(m_chart,LV_CHART_AXIS_PRIMARY_Y,20,80);

    lv_chart_set_div_line_count(m_chart,10,10);

    lv_chart_set_axis_tick(m_chart, LV_CHART_AXIS_PRIMARY_Y, 2, 1, 7, 2, true, 50);
    lv_chart_set_axis_tick(m_chart, LV_CHART_AXIS_SECONDARY_Y, 2, 1, 6, 4, true, 50);

    lv_chart_set_point_count(m_chart, 100);


    /* Add two data series */
    m_chart_ser_temperature = lv_chart_add_series(m_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    m_chart_ser_humidity = lv_chart_add_series(m_chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_SECONDARY_Y);
    m_chart_ser_target = lv_chart_add_series(m_chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_all_value(m_chart,m_chart_ser_target,m_target);

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

    /**
     * @brief Create target temperature spinbox
     * 
     */    
    char txt[20];
    sprintf(txt,"%2d °C",m_target);
    
    m_lbl_target = lv_label_create(m_screen);
    lv_label_set_long_mode(m_lbl_target, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_target, txt);
    lv_obj_set_style_text_align(m_lbl_target, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_target, 100);
    lv_obj_align_to(m_lbl_target,m_chart,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    lv_obj_set_style_text_font(m_lbl_target,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_target,lv_color_make(0,255,0),0);
    
    lv_obj_t * btn = lv_btn_create(m_screen);
    lv_obj_set_size(btn, 40, 30);
    lv_obj_align_to(btn, m_lbl_target, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, BTN_temp_inc, LV_EVENT_CLICKED,  NULL);
    lv_obj_add_event_cb(btn, BTN_temp_inc, LV_EVENT_LONG_PRESSED_REPEAT,  NULL);

    btn = lv_btn_create(m_screen);
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
    m_lbl_timer_h = lv_label_create(m_screen);
    lv_obj_set_style_text_align(m_lbl_timer_h, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(m_lbl_timer_h, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_timer_h, txt);
    lv_obj_set_style_text_align(m_lbl_timer_h, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_timer_h, 50);
    lv_obj_align_to(m_lbl_timer_h,m_lbl_target,LV_ALIGN_OUT_BOTTOM_MID,-25,10);
    lv_obj_set_style_text_font(m_lbl_timer_h,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_timer_h,lv_color_make(0,255,0),0);
    
    sprintf(txt,"%02d",m_timer_m);
    m_lbl_timer_m = lv_label_create(m_screen);
    lv_obj_set_style_text_align(m_lbl_timer_m, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(m_lbl_timer_m, LV_LABEL_LONG_WRAP);
    lv_label_set_text(m_lbl_timer_m, txt);
    lv_obj_set_style_text_align(m_lbl_timer_m, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(m_lbl_timer_m, 50);
    lv_obj_align_to(m_lbl_timer_m,m_lbl_target,LV_ALIGN_OUT_BOTTOM_MID,+25,10);
    lv_obj_set_style_text_font(m_lbl_timer_m,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(m_lbl_timer_m,lv_color_make(0,255,0),0);    
    
    lv_obj_t * lbl = lv_label_create(m_screen);
    lv_label_set_text(lbl, ":");
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(lbl, 10);
    lv_obj_align_to(lbl,m_lbl_target,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    lv_obj_set_style_text_font(lbl,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(lbl,lv_color_make(0,255,0),0);    

    btn = lv_btn_create(m_screen);
    lv_obj_set_size(btn, 40, 30);
    lv_obj_align_to(btn, m_lbl_timer_m, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, BTN_timer_m_inc, LV_EVENT_CLICKED,  NULL);
    lv_obj_add_event_cb(btn, BTN_timer_h_inc, LV_EVENT_LONG_PRESSED_REPEAT,  NULL);

    btn = lv_btn_create(m_screen);
    lv_obj_set_size(btn, 40, 30);
    lv_obj_align_to(btn, m_lbl_timer_h, LV_ALIGN_OUT_LEFT_MID, -10, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, BTN_timer_m_dec, LV_EVENT_CLICKED, NULL);    
    lv_obj_add_event_cb(btn, BTN_timer_h_dec, LV_EVENT_LONG_PRESSED_REPEAT, NULL); 


    /**
     * @brief Create START button
     * 
     */
    btn = lv_btn_create(m_screen);
    //lv_obj_add_event_cb(btn, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_width(btn,200);
    lbl = lv_label_create(btn);    
    lv_label_set_text(lbl, "Start");
    lv_obj_center(lbl);
    lv_obj_set_style_text_font(lbl,&lv_font_montserrat_28,0);
    lv_obj_set_style_text_color(lbl,lv_color_make(255,0,0),0);


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
    char txt[20];
    lv_chart_set_next_value(m_chart, m_chart_ser_temperature, temperature);
    lv_chart_set_next_value(m_chart, m_chart_ser_humidity, humidity);

    sprintf(txt,"% 3.1f °C",temperature);
    lv_label_set_text(m_lbl_temperature, txt);
    sprintf(txt,"% 3.1f %%",humidity);
    lv_label_set_text(m_lbl_humidity, txt);
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
                if(dsc_chart->text) dsc_chart->label_dsc->color = lv_color_make(255, 0, 0);
            }
            else if (dsc_chart->id == LV_CHART_AXIS_SECONDARY_Y) {
                if(dsc_chart->text) dsc_chart->label_dsc->color = lv_color_make(0, 0, 255);
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
    lv_chart_set_all_value(m_pMainMenu->m_chart,m_pMainMenu->m_chart_ser_target,m_pMainMenu->m_target);
}

void MainMenu::BTN_temp_dec(lv_event_t *e){
    char txt[20];    
    m_pMainMenu->m_target--;
    if( (m_pMainMenu->m_target) < 20) m_pMainMenu->m_target = 20;
    sprintf(txt,"%2d °C",m_pMainMenu->m_target);
    lv_label_set_text(m_pMainMenu->m_lbl_target,txt);
    lv_chart_set_all_value(m_pMainMenu->m_chart,m_pMainMenu->m_chart_ser_target,m_pMainMenu->m_target);
}

void MainMenu::BTN_timer_h_inc(lv_event_t *e){
    char txt[20];    
    m_pMainMenu->m_timer_h++;
    if( (m_pMainMenu->m_timer_h) > 6){
        m_pMainMenu->m_timer_h = 6;
    }
    sprintf(txt,"%02d",m_pMainMenu->m_timer_h);
    lv_label_set_text(m_pMainMenu->m_lbl_timer_h,txt);  
}

void MainMenu::BTN_timer_h_dec(lv_event_t *e){
    char txt[20];    
    m_pMainMenu->m_timer_h--;
    if( (m_pMainMenu->m_timer_h) < 0){
        m_pMainMenu->m_timer_h = 0;
    }
    sprintf(txt,"%02d",m_pMainMenu->m_timer_h);
    lv_label_set_text(m_pMainMenu->m_lbl_timer_h,txt);  
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
