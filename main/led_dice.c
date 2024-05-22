#include "led_dice.h"
#include "led_strip.h"
#include "inttypes.h"
#include "esp_log.h"

#define RNG_DATA_REG 0x600260B0
#define LOW_POWER_MANAGEMENT_BASE 0x60008000
#define RTC_CNTL_CLK_CONF_REG (LOW_POWER_MANAGEMENT_BASE + 0x0070)
#define RTC_CNTL_DIG_FOSC_EN 10 // This bit in the RTC_CNTL_CLK_CONF_REG needs to be enabled for RTC CLK, to get RNG

volatile uint32_t *RNG_DATA_REG_p = (uint32_t *)RNG_DATA_REG;

static return_t init_RNG()
{
    uint32_t *RTC_CNTL_CLK_CONF_REG_p = (uint32_t *)RTC_CNTL_CLK_CONF_REG;
    *RTC_CNTL_CLK_CONF_REG_p |= (uint32_t)(1 << RTC_CNTL_DIG_FOSC_EN);
    return success;
}

#define LED_GPIO 8
#define MAX_LEDS 25

bool g_leds_configured = false;

static const char *TAG = "LED_CONFIG";

led_strip_handle_t led_strip; // Wie kann i des anders Lösen?? kann ich die nicht in der funktion configure definieren???

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

return_t init_dice()
{
    if (!g_leds_configured)
    {
        init_RNG();
        g_leds_configured = true;

        // led_strip_handle_t led_strip;

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
    else
    {
        ESP_LOGE(TAG, "LEDs already configured!");
        return already_configured;
    }
}

return_t set_led(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t delay = 1000;

    for (int i = 0; i < 6; i++)
    {
        led_strip_clear(led_strip);
        for (int j = 0; j < 25; j++)
        {
            if (dice_number_pixel_indices[i][j])
            {
                led_strip_set_pixel(led_strip, j, r, g, b);
            }
        }
        led_strip_refresh(led_strip);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }

    while (1)
    {
        uint8_t random_number = (*RNG_DATA_REG_p) % 6;
        led_strip_clear(led_strip);
        for (int j = 0; j < 25; j++)
        {
            if (dice_number_pixel_indices[random_number][j])
            {
                led_strip_set_pixel(led_strip, j, r, g, b);
            }
        }
        led_strip_refresh(led_strip);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }

    return success;
}
