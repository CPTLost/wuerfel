#include "led_dice.h"

#include "led_strip.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "rng.h"
#include "button.h"

// This Code is primarily made for Patricks Dev Board

#define LED_GPIO 8
#define MAX_LEDS 25

bool g_leds_configured = false;

static bool g_roll_dice = false;
static bool g_cheat = false;

void rolling_button_callback(void)
{
    g_roll_dice = true;
}
void cheating_button_callback(void)
{
    g_cheat = true;
}

static led_strip_handle_t led_strip;

uint8_t dice_number_pixel_indices[6][MAX_LEDS] = {
    {0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0,
     0, 1, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 1, 0,
     0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0,
     0, 1, 0, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 0, 1, 0,
     0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0,
     0, 1, 0, 1, 0,
     0, 0, 0, 0, 0,
     0, 1, 0, 1, 0,
     0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0,
     0, 1, 0, 1, 0,
     0, 0, 1, 0, 0,
     0, 1, 0, 1, 0,
     0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 0, 0, 0, 0}};

return_t roll_dice()
{
    if (g_roll_dice == false)
    {
        return rolling_dice_button_not_pressed;
    }

    uint32_t delay = 50;
    uint8_t dice_loops = 3;
    for (uint8_t k = 0; k < dice_loops; k++)
    {
        for (int i = 0; i < ((k == dice_loops - 1 && g_cheat == false) ? (get_rng_value() % 6) : (6)); i++)
        {

            led_strip_clear(led_strip);
            for (int j = 0; j < 25; j++)
            {
                if (dice_number_pixel_indices[i][j])
                {
                    led_strip_set_pixel(led_strip, j, 25, 25, 25);
                }
            }
            led_strip_refresh(led_strip);
            vTaskDelay(delay / portTICK_PERIOD_MS);
            delay += 20;
        }
    }
    g_roll_dice = false;
    if (g_cheat == true)
    {
        g_cheat = false;
    }
    return success;
}

return_t init_dice()
{
    init_RNG();
    configure_button_for_led_dice(9, button_for_rolling_dice);
    configure_button_for_led_dice(2, button_to_cheat);
    g_leds_configured = true;

    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,               // The GPIO that connected to the LED strip's data line
        .max_leds = MAX_LEDS,                     // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,    // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,           // whether to enable the DMA feature
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    return success;
}