#include "button.h"

#include <inttypes.h>
#include <driver/gpio.h>
#include "esp_log.h"

#include "return_type.h"

// #define LEFT_BTN_GPIO 9
// #define RIGHT_BTN_GPIO 2

static const char *TAG = "BUTTON";

static void button_for_rolling_dice_isr_handler()
{
    ESP_EARLY_LOGE(TAG, "\nISR\n");
    g_roll_dice = true;
    ESP_EARLY_LOGE(TAG, "\nISR Finished\n");
}

static void button_for_cheating_isr_handler()
{
    ESP_EARLY_LOGE(TAG, "\nISR: cheating\n");
    g_cheat = true;
    ESP_EARLY_LOGE(TAG, "\nISR: cheating Finished\n");
}

return_t configure_button_for_rolling_dice(uint8_t GPIO_BUTTON_PIN)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1 << GPIO_BUTTON_PIN,
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_BUTTON_PIN, button_for_rolling_dice_isr_handler, NULL);
    return success;
}

return_t configure_button_for_cheating(uint8_t GPIO_BUTTON_PIN)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1 << GPIO_BUTTON_PIN,
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_BUTTON_PIN, button_for_cheating_isr_handler, NULL);
    return success;
}