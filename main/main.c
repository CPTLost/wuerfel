#include <led_dice.h>

#define LEFT_BTN_GPIO 9
#define RIGHT_BTN_GPIO 2

void app_main(void)
{
    init_dice();
    while (1)
    {
        set_led();
    }
}
