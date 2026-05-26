#include "servo.h"

void servo_cb(void *args)
{
    servo_cb_args_t *servo_cb_args = (servo_cb_args_t *)args;
    iot_servo_write_angle(LEDC_LOW_SPEED_MODE, *servo_cb_args->channel, *servo_cb_args->controls->angle);
    ESP_LOGI(TAG, "cb");
}

void servo_init(void *vParameters)
{
    ESP_LOGI(TAG, "hello");
    float *angle = (float *)vParameters;
    servo_cb_args_t *servo_cb_args = (servo_cb_args_t *)vParameters;
    ESP_LOGI(TAG, "channel_number: %d", servo_cb_args->servo_config->channel_number);
    ESP_ERROR_CHECK(iot_servo_init(LEDC_LOW_SPEED_MODE, servo_cb_args->servo_config));

    while (1)
    {
        // ESP_LOGI(TAG, "channel: %d", (int)servo_cb_args->servo_config->channels.ch);
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, *servo_cb_args->controls->angle);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // esp_timer_handle_t timer;
    // esp_timer_create_args_t args = {
    //     .callback = servo_cb,
    //     .arg = servo_cb_args,
    //     .dispatch_method = ESP_TIMER_TASK,
    //     .name = "servo"};

    // ESP_ERROR_CHECK(esp_timer_create(&args, &timer));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(timer, INTERVAL_US));
}