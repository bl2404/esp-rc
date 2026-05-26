#include "motor.h"

static const char *TG = "motor";

// static void dshot_timer_cb(void *arg)
// {
//     timer_handle_t *timer_handle = (timer_handle_t *)arg;

//     ESP_ERROR_CHECK(rmt_transmit(*timer_handle->esc_chan, *timer_handle->dshot_encoder, timer_handle->controls->throttle, sizeof(*timer_handle->controls->throttle), timer_handle->tx_config));
//     ESP_ERROR_CHECK(rmt_disable(*timer_handle->esc_chan));
//     ESP_ERROR_CHECK(rmt_enable(*timer_handle->esc_chan));
// }

void motor_init(void *vParameters)
{
    crsf_control_state_t *controls = (crsf_control_state_t *)vParameters;

    ESP_LOGI(TAG, "Create RMT TX channel");
    rmt_channel_handle_t esc_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select a clock that can provide needed resolution
        .gpio_num = DSHOT_ESC_GPIO_NUM,
        .mem_block_symbols = 64,
        .resolution_hz = DSHOT_ESC_RESOLUTION_HZ,
        .trans_queue_depth = 10, // set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &esc_chan));

    ESP_LOGI(TAG, "Install Dshot ESC encoder");
    rmt_encoder_handle_t dshot_encoder = NULL;
    dshot_esc_encoder_config_t encoder_config = {
        .resolution = DSHOT_ESC_RESOLUTION_HZ,
        .baud_rate = 300000, // DSHOT300 protocol
        .post_delay_us = 20, // extra delay between each frame
    };
    ESP_ERROR_CHECK(rmt_new_dshot_esc_encoder(&encoder_config, &dshot_encoder));

    ESP_LOGI(TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(esc_chan));

    rmt_transmit_config_t tx_config = {
        .loop_count = -1, // infinite loop
    };
    dshot_esc_throttle_t throttle = {
        .throttle = 0,
        .telemetry_req = false, // telemetry is not supported in this example
    };

    ESP_LOGI(TAG, "Start ESC by sending zero throttle for a while...");
    ESP_ERROR_CHECK(rmt_transmit(esc_chan, dshot_encoder, &throttle, sizeof(throttle), &tx_config));

    vTaskDelay(pdMS_TO_TICKS(5000));

    while (1)
    {
        ESP_LOGI(TAG, "channel: %d", *controls->throttle);
        if (*controls->throttle == 48)
        {
            *controls->throttle = 0;
        }
        ESP_ERROR_CHECK(rmt_transmit(esc_chan, dshot_encoder, controls->throttle, sizeof(*controls->throttle), &tx_config));
        ESP_ERROR_CHECK(rmt_disable(esc_chan));
        ESP_ERROR_CHECK(rmt_enable(esc_chan));
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // esp_timer_handle_t timer;

    // timer_handle_t timer_handle = {
    //     .esc_chan = &esc_chan,
    //     .dshot_encoder = &dshot_encoder,
    //     .tx_config = &tx_config,
    //     .timer = &timer,
    //     .controls = controls};

    // esp_timer_create_args_t args = {
    //     .callback = dshot_timer_cb,
    //     .arg = &timer_handle,
    //     .dispatch_method = ESP_TIMER_TASK, // safe default
    //     .name = "dshot_timer"};

    // ESP_ERROR_CHECK(esp_timer_create(&args, &timer));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(timer, INTERVAL_US));

    vTaskDelete(NULL);
}