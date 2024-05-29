#include "rng.h"

#define RNG_DATA_REG 0x600260B0
#define LOW_POWER_MANAGEMENT_BASE 0x60008000
#define RTC_CNTL_CLK_CONF_REG (LOW_POWER_MANAGEMENT_BASE + 0x0070)
#define RTC_CNTL_DIG_FOSC_EN 10 // This bit in the RTC_CNTL_CLK_CONF_REG needs to be enabled for RTC CLK, to get RNG

volatile uint32_t *RNG_DATA_REG_p = (uint32_t *)RNG_DATA_REG;

return_t init_RNG()
{
    uint32_t *RTC_CNTL_CLK_CONF_REG_p = (uint32_t *)RTC_CNTL_CLK_CONF_REG;
    *RTC_CNTL_CLK_CONF_REG_p |= (uint32_t)(1 << RTC_CNTL_DIG_FOSC_EN);
    return success;
}

uint32_t get_rng_value()
{
    return *RNG_DATA_REG_p;
}