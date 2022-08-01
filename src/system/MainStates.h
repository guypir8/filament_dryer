#ifndef _MAIN_STATES_
#define _MAIN_STATES_

typedef enum{

    /* Menu principali */
    MAIN_MENU,
    START,
    RAMPUP,
    PID,
    ALARM,

} MAIN_STATE;

extern MAIN_STATE main_state;

#endif