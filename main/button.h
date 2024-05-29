#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <inttypes.h>

#include "return_type.h"

typedef enum
{
    button_for_rolling_dice,
    button_to_cheat,
} button_t;

return_t configure_button_for_led_dice(uint8_t GPIO_BUTTON_PIN, button_t button_type);

#endif