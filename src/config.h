/**
 * @file config.h
 * @author GuyPir8 (guypir8@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2022-08-01
 * 
 * @copyright Copyright (c) 2022
 * 
 * v1.0 :   First emission
 */

/**
 * @brief PID CONTROLS
 * 
 */
#define PID_KP 160
#define PID_KI 0.06
#define PID_KD 50

#define PID_REFRESH_RATE 200
#define PID_INTEGRAL_THRESHOLD 1
#define PID_WARMUP_THRESHOLD 1

/**
 * @brief RELE CONTROL
 * 
 */
#define RELE_PIN 26
#define RELE_PWM_PERIOD 30

/**
 * @brief DHT22 SENSOR
 * 
 */
#define DHT22_PIN 32

/**
 * @brief ALARMS
 * 
 */
#define ALARM_OVERTEMP_TEMP 80
#define ALARM_OVERTEMP_TIMEOUT_SEC 5u
#define ALARM_HEATERFAIL_TIMEOUT_SEC 7200u
#define ALARM_HEATERFAIL_THRESHOLD 3

/**
 * @brief CHART CONFIGURATION
 * 
 */
#define CHART_POINTS 100