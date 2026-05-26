#include <dshot_esc_encoder.h>
#include <esp_timer.h>
#include "driver/rmt_tx.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "crsf.h"

#define DSHOT_ESC_RESOLUTION_HZ 40000000 // 40MHz resolution, DSHot protocol needs a relative high resolution
#define DSHOT_ESC_GPIO_NUM 3

#define INTERVAL_US 500

typedef struct
{
    rmt_channel_handle_t *esc_chan;
    rmt_encoder_handle_t *dshot_encoder;
    rmt_transmit_config_t *tx_config;
    esp_timer_handle_t *timer;
    crsf_control_state_t *controls;
} timer_handle_t;

void motor_init(void *vParameters);