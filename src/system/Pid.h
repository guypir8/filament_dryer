#ifndef _PID_
#define _PID_

class Pid {

    public:
        Pid(float target, float diff);
        float PID_control(float real_temp);
        bool ramp_up(float real_temp); 
        void setPid(float p, float i, float d);

    private:
        unsigned long elapsedTime;
        unsigned long prev_time;
        unsigned long refresh_rate;

        float Setpoint;
        float kp;
        float kd;
        float ki;
        float prev_pid_error;
        float SetpointDiff;

        float PID_p;
        float PID_d;
        float PID_i;
};

#endif