#include "button.h"

#include <driver/gpio.h>
#include "esp_log.h"

static bool isr_installed = false;

void rolling_button_callback(void);
void cheating_button_callback(void);

void button_for_rolling_dice_isr_handler()
{
    rolling_button_callback();
}

void button_for_cheating_isr_handler()
{
    cheating_button_callback();
}

return_t configure_button_for_led_dice(uint8_t GPIO_BUTTON_PIN, button_t button_type)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1 << GPIO_BUTTON_PIN,
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&io_conf);

    if (isr_installed == false)
    {
        gpio_install_isr_service(0);
        isr_installed = true;
    }

    if (button_type == button_for_rolling_dice)
    {
        gpio_isr_handler_add(GPIO_BUTTON_PIN, button_for_rolling_dice_isr_handler, NULL);
    }
    else if (button_type == button_to_cheat)
    {
        gpio_isr_handler_add(GPIO_BUTTON_PIN, button_for_cheating_isr_handler, NULL);
    }
    return success;
}

__attribute__((weak)) void rolling_button_callback(void) {}
__attribute__((weak)) void cheating_button_callback(void) {}