#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <ESP_CRSF.h>

#define INTERVAL_US 500

typedef struct
{
    int *throttle;
    float *angle;
} crsf_control_state_t;

typedef struct
{
    crsf_config_t *crsf_config;
    crsf_channels_t *channels;
    crsf_control_state_t *controls;
} crsf_args_t;

void crsf_init(void *vParameters);