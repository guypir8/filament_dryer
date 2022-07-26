#include "style.h"

lv_style_t style_screen;
lv_style_t style_btnDef;
lv_style_t style_btnPr;

void style_init(){            
    /* Screen */
    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, lv_color_make(0,0,0));

    /* Image button default */
    lv_style_init(&style_btnDef);
    lv_style_set_text_color(&style_btnDef, lv_color_white());
        
    /* Image button pressed */
    lv_style_init(&style_btnPr);
    lv_style_set_img_recolor_opa(&style_btnPr, LV_OPA_30);
    lv_style_set_img_recolor(&style_btnPr, lv_color_black());

}