#ifndef _API_H_
#define _API_H_
#include "style.h"
#include "system/UTimer.h"

lv_obj_t * api_create_screen();
lv_obj_t * api_create_menu_button(lv_obj_t * parent, const lv_img_dsc_t * img, int pos);
lv_obj_t * api_create_form_label(lv_obj_t * parent, int left, int top, int width, int height, const char * text);
lv_obj_t * api_create_form_field(lv_obj_t * parent, int left, int top, int width, int height, const char * text);

void beep();
void jingle_good();
void jingle_bad();
void keypressed();

extern UTimer keypressedTimeout;
#endif