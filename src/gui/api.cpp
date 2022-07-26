#include "api.h"
#include "lvgl.h"
#include <Arduino.h>

static SemaphoreHandle_t sound_mutex = xSemaphoreCreateMutex();
UTimer keypressedTimeout;

/**
 * @brief Create screen
 * 
 * @return lv_obj_t* screen object
 */
lv_obj_t * api_create_screen(){
    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_add_style(screen,&style_screen,0);
    return screen;
}

/**
 * @brief Create menu buttons
 * 
 * @param parent parent screen
 * @param img button image
 * @param pos position 0-11
 * @return lv_obj_t* button object
 */
lv_obj_t * api_create_menu_button(lv_obj_t * parent, const lv_img_dsc_t * img, int pos){
    uint32_t w = img->header.w;
    uint32_t h = img->header.h;
    uint32_t l = (80-w)/2;
    uint32_t t = (80-h)/2;

    lv_obj_t * button = lv_imgbtn_create(parent);    
    lv_obj_set_size(button, w, h);
    lv_imgbtn_set_src(button, LV_IMGBTN_STATE_RELEASED, NULL, img, NULL);
    lv_obj_add_style(button, &style_btnDef, 0);
    lv_obj_add_style(button, &style_btnPr, LV_STATE_PRESSED);
    
    switch(pos){
        /* Prima fila */
        case 0: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 0+l, 0+t); break;
        case 1: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 80+l, 0+t); break;
        case 2: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 160+l, 0+t); break;
        case 3: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 240+l, 0+t); break;
        /* Seconda fila */
        case 4: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 0+l, 80+t); break;
        case 5: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 80+l, 80+t); break;
        case 6: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 160+l, 80+t); break;
        case 7: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 240+l, 80+t); break;
        /* Terza fila */        
        case 8: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 0+l, 160+t); break;
        case 9: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 80+l, 160+t); break;
        case 10: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 160+l, 160+t); break;
        case 11: lv_obj_align(button, LV_ALIGN_TOP_LEFT, 240+l, 160+t); break;        
    }
    return button;
}

void beepTask(void*arg){
    xSemaphoreTake(sound_mutex, portMAX_DELAY);
    ledcWriteNote(0,NOTE_C,5);
    delay(50);
    ledcWrite(0,0);
    xSemaphoreGive(sound_mutex);
    vTaskDelete( NULL );
}

void jingleGoodTask(void*arg){
    xSemaphoreTake(sound_mutex, portMAX_DELAY);
    delay(100);
    ledcWriteNote(0,NOTE_C,5);
    delay(90);
    ledcWriteNote(0,NOTE_D,5);
    delay(90);
    ledcWriteNote(0,NOTE_F,5);
    delay(250);
    ledcWrite(0,0);
    xSemaphoreGive(sound_mutex);
    vTaskDelete( NULL );
}

void jingleBadTask(void*arg){
    xSemaphoreTake(sound_mutex, portMAX_DELAY);
    delay(100);
    ledcWriteNote(0,NOTE_D,3);
    delay(600);   
    ledcWrite(0,0); 
    xSemaphoreGive(sound_mutex);
    vTaskDelete( NULL );
}

/**
 * @brief Keyboard beep
 * 
 */
void beep(){
    keypressedTimeout.reset();
        xTaskCreate(beepTask,
                    "beep",
                    20000,
                    NULL,
                    1,
                    NULL);
}
void keypressed(){
    keypressedTimeout.reset();
}

/**
 * @brief Good code jingle
 * 
 */
void jingle_good(){
    xTaskCreate(jingleGoodTask,
                "jingleGoodTask",
                20000,
                NULL,
                1,
                NULL);

}

/**
 * @brief Bad code jingle
 * 
 */
void jingle_bad(){
    xTaskCreate(jingleBadTask,
                "jingleBadTask",
                20000,
                NULL,
                1,
                NULL);

}
