#ifndef _BASE_MENU_
#define _BASE_MENU_

#include "lvgl.h"
#include "style.h"
#include "system/MainStates.h"
#include "api.h"

extern bool doBeep;

class BaseMenu{
    public:
        BaseMenu();

    protected:
        lv_obj_t * m_screen;
        
};

#endif