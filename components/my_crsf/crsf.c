#include "crsf.h"

#define LINEAR_INTERPOLATE(type, x1, y1, x2, y2, x) ((type)(y1) + ((type)(y2) - (type)(y1)) * ((type)(x) - (type)(x1)) / ((type)(x2) - (type)(x1)))

static float calc_servo_angle(int channel_state)
{
    return LINEAR_INTERPOLATE(float, 174, 0, 1811, 180, (float)channel_state);
}

static int calc_throttle_level(int channel_state)
{
    return (int)LINEAR_INTERPOLATE(int, 174, 48, 1811, 2047, channel_state);
}

void callback(void *args)
{
    crsf_args_t *crsf_args = (crsf_args_t *)args;
    CRSF_receive_channels(crsf_args->channels);
    *crsf_args->controls->angle = calc_servo_angle(crsf_args->channels->ch1);
    *crsf_args->controls->throttle = calc_throttle_level(crsf_args->channels->ch3);
}

void crsf_init(void *vParameters)
{
    crsf_control_state_t *controls = (crsf_control_state_t *)vParameters;
    crsf_config_t config = {
        .uart_num = UART_NUM_1,
        .tx_pin = 4,
        .rx_pin = 5};

    CRSF_init(&config);

    crsf_channels_t channels = {0};

    crsf_args_t crsf_args = {
        .channels = &channels,
        .controls = controls,
        .crsf_config = &config};

    while (1)
    {
        CRSF_receive_channels(&channels);
        *controls->angle = calc_servo_angle(channels.ch1);
        *controls->throttle = calc_throttle_level(channels.ch3);
        // ESP_LOGI(TAG, "angle: %f", *controls->angle);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // esp_timer_handle_t timer;
    // esp_timer_create_args_t args = {
    //     .callback = callback,
    //     .arg = &crsf_args,
    //     .dispatch_method = ESP_TIMER_TASK,
    //     .name = "servo"};

    // ESP_ERROR_CHECK(esp_timer_create(&args, &timer));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(timer, INTERVAL_US));
}