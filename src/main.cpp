#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "MainApp.h"
#include "system/UTimer.h"

#define U_2SEC 1000000

#define _ORIENTATION_ VERTICAL

/* TFT */
#if _ORIENTATION_ == VERTICAL
    #define _SCREEN_WIDTH_ 240
    #define _SCREEN_HEIGHT_ 320
#else
    #define _SCREEN_WIDTH_ 320
    #define _SCREEN_HEIGHT_ 240
#endif

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[_SCREEN_WIDTH_ * 10];
TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

/* Timers */
UTimer debugTimer;

/* Main APP */
MainApp * mainApp = NULL;

/* WiFi */
UTimer wifiTimer;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{

    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void hardwareInit(){
    /*
    * Disable all CS
    */
    pinMode(22, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(2, OUTPUT);    

    /*
    * Switch on backlit
    */
    /*
    pinMode(15, OUTPUT);
    digitalWrite(15, LOW);
    delay(1);    
    */
    ledcAttachPin(15,4);
    ledcSetup(4,5000,8);
    ledcWrite(4,0);

    lv_init();
    tft.begin();        /* TFT init */    

    #if _ORIENTATION_ == VERTICAL
        tft.setRotation(0); 
        uint16_t calData[5] = {379, 3486, 264, 3405, 4};
    #else
        tft.setRotation(1); /* Landscape orientation, flipped */
        uint16_t calData[5] = {379, 3486, 264, 3405, 7};
    #endif

    
    tft.setTouch(calData);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, _SCREEN_WIDTH_ * 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    /*Change the following line to your display resolution*/
    disp_drv.hor_res = _SCREEN_WIDTH_;
    disp_drv.ver_res = _SCREEN_HEIGHT_;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

void setup()
{
    Serial.begin(115200);

    hardwareInit();
    style_init();
    mainApp = new MainApp();

    /* Timers */
    debugTimer.reset();

    /* Beep */
    ledcAttachPin(21,0);
    ledcSetup(0,1000,8);
    ledcWrite(0,0);

}

void loop()
{    
    lv_timer_handler(); /* let the GUI do its work */
    mainApp->run();
    delay(5);     
}
