/* main.c — for NUCLEO-G491RE (STM32G491RE) with libopencm3 built for stm32/g4 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include "core/system.h"

#define LED_PORT 0x48000000
#define LED_PIN  1 << 5

static void gpio_setup(void)
{
    /* enable GPIOA clock and set PA5 as output */
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

int main(void)
{
    system_setup();
    gpio_setup();

    uint64_t start_time = system_get_ticks();

    while (1) {
        if (system_get_ticks() - start_time >= 1000) {
            gpio_toggle(LED_PORT, LED_PIN);
            start_time = system_get_ticks();
        }
    }

    /* never reached */
    return 0;
}
