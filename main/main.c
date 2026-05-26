#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include "motor.h"
#include "servo.h"
#include "crsf.h"

static const char *TG = "example";

#define LINEAR_INTERPOLATE(type, x1, y1, x2, y2, x) ((type)(y1) + ((type)(y2) - (type)(y1)) * ((type)(x) - (type)(x1)) / ((type)(x2) - (type)(x1)))

float calc_servo_angle(int channel_state)
{
    return LINEAR_INTERPOLATE(float, 174, 0, 1811, 180, (float)channel_state);
}

int calc_throttle_level(int channel_state)
{
    return (int)LINEAR_INTERPOLATE(int, 174, 48, 1811, 2047, channel_state);
}

void app_main(void)
{

    servo_config_t servo_cfg = {
        .max_angle = 180,
        .min_width_us = 500,
        .max_width_us = 2500,
        .freq = 50,
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = {
                SERVO_GPIO,
            },
            .ch = {
                LEDC_CHANNEL_0,
            },
        },
        .channel_number = 1,
    };

    crsf_control_state_t *control = malloc(sizeof(crsf_control_state_t));
    control->angle = malloc(sizeof(float));
    control->throttle = malloc(sizeof(int));
    *control->angle = 0;
    *control->throttle = 0;
    servo_cb_args_t *servo_args = malloc(sizeof(servo_cb_args_t));
    servo_args->channel = malloc(sizeof(int));
    *servo_args->channel = 0;
    servo_args->servo_config = &servo_cfg;
    servo_args->controls = control;

    xTaskCreate(crsf_init, "crsf", configMINIMAL_STACK_SIZE * 5, (void *)control, 2, NULL);
    xTaskCreate(servo_init, "servo", configMINIMAL_STACK_SIZE * 5, (void *)servo_args, 2, NULL);
    xTaskCreate(motor_init, "motor", configMINIMAL_STACK_SIZE * 5, (void *)control, 2, NULL);

    //  float angle = 20;
    //  int throttle = 0;

    // Initialize the servo
}
