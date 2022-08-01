#include <Arduino.h>
#include "Pid.h"

Pid::Pid(float target, float diff){
    Setpoint = target;
    SetpointDiff = diff;

    kp=160;    //2.5
    ki=0.06;    //0.06
    kd=50;     // 0.8   

    PID_p = 0;
    PID_d = 0;
    PID_i = 0;

    refresh_rate = 200;

    Serial.printf("PID TARGET=%f\n",target);
}

void Pid::setPid(float p, float i, float d){
    kp=p;
    ki=i;
    kd=d;
}

float Pid::PID_control(float real_temp)
{
    float now_pid_error;
    float PID_total;

    PID_total = -1;

    elapsedTime = millis() - prev_time;
    if (elapsedTime > refresh_rate)
    {

        now_pid_error = Setpoint - real_temp;

        PID_p = kp * now_pid_error;
        PID_d = kd * ((now_pid_error - prev_pid_error) / (float)refresh_rate);

        if (-1 < now_pid_error && now_pid_error < 1)
        {
            PID_i = PID_i + (ki * now_pid_error);
            Serial.printf("PID_i=%f i=%f\n",PID_i,ki * now_pid_error);
        }
        else
        {
            PID_i = 0;
        }
        Serial.printf("ERR=%f P=%f I=%f D=%f\n",now_pid_error, PID_p , PID_i , PID_d);

        PID_total = PID_p + PID_i + PID_d;
        PID_total = map(PID_total, 0, 150, 0, 255);


        if (PID_total < 0)
        {
            PID_total = 0;
        }
        if (PID_total > 255)
        {
            PID_total = 255;
        }


        prev_time = millis();
        prev_pid_error = now_pid_error;
    }

    return PID_total;
}

bool Pid::ramp_up(float real_temp){  
  bool heater = true;
  elapsedTime = millis() - prev_time; 
  if(elapsedTime > refresh_rate){    
    if(real_temp < (Setpoint - SetpointDiff)){
        heater = true;
    }
    else
    {
      heater = false;
    }
    prev_time = millis();
  }
  return heater;
}
