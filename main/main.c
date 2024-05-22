#include <stdio.h>
#include "led_dice.h"

void app_main(void)
{
    init_dice();
    set_led(0, 25, 25, 25);
}
