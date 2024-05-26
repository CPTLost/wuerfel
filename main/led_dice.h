#ifndef LED_DICE_H
#define LED_DICE_H

#include "inttypes.h"
#include "return_type.h"

return_t init_dice();
return_t set_led(uint8_t led, uint8_t r, uint8_t g, uint8_t b);

#endif