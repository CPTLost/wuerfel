#ifndef RNG_H
#define RNG_H

#include "return_type.h"
#include <inttypes.h>

return_t init_RNG();
uint32_t get_rng_value();

#endif