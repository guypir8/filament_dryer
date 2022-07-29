#include <Arduino.h>
#include "Pid.h"

Pid::Pid(float target, float diff){
    Setpoint = target;
    SetpointDiff = diff;

    kp=160;     //Mine was 2.5
    ki=0.1;    //Mine was 0.06
    kd=10;     //Mine was 0.8   

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
        // 1. We get the temperature and calculate the error
        now_pid_error = Setpoint - real_temp;

        // 2. We calculate PID values
        PID_p = kp * now_pid_error;
        PID_d = kd * ((now_pid_error - prev_pid_error) / refresh_rate);
        // 2.2 Decide if we apply I or not. Only when error is very small
        if (-3 < now_pid_error && now_pid_error < 3)
        {
            PID_i = PID_i + (ki * now_pid_error);
            Serial.printf("PID_i=%f i=%f\n",PID_i,ki * now_pid_error);
        }
        else
        {
            PID_i = 0;
        }
        Serial.printf("ERR=%f P=%f I=%f D=%f\n",now_pid_error, PID_p , PID_i , PID_d);
        // 3. Calculate and map total PID value
        PID_total = PID_p + PID_i + PID_d;
        PID_total = map(PID_total, 0, 150, 0, 255);

        // 4. Set limits for PID values
        if (PID_total < 0)
        {
            PID_total = 0;
        }
        if (PID_total > 255)
        {
            PID_total = 255;
        }

        // 7. Save values for next loop
        prev_time = millis();           // Store time for next loop
        prev_pid_error = now_pid_error; // Store error for next loop
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
}//End of ramp_up loop
