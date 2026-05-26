#pragma once

#include <iot_servo.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_timer.h>
#include "crsf.h"

#define INTERVAL_US 500
#define SERVO_GPIO 9

typedef struct
{
    servo_config_t *servo_config;
    int *channel;
    crsf_control_state_t *controls;
} servo_cb_args_t;

void servo_init(void *vParameters);