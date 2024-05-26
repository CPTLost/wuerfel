#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <inttypes.h>

#include "return_type.h"

bool g_roll_dice = false;
bool g_cheat = false;

return_t configure_button_for_rolling_dice(uint8_t GPIO_BUTTON_PIN);
return_t configure_button_for_cheating(uint8_t GPIO_BUTTON_PIN);

#endif