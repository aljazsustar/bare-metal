/* main.c — for NUCLEO-G491RE (STM32G491RE) with libopencm3 built for stm32/g4 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

#define LED_PORT 0x48000000
#define LED_PIN  1 << 5

static void gpio_setup(void)
{
    /* enable GPIOA clock and set PA5 as output */
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

/* Simple busy-wait delay (approx; not precise) */
static void delay_ms_approx(uint32_t ms)
{
    volatile uint32_t n = ms * 1200; /* tuned for roughly 1 ms at 96 MHz */
    while (n--) __asm__("nop");
}

int main(void)
{
    /* Use libopencm3 G4 clock helper to set 96 MHz from HSI16 */
    /* This uses rcc_hsi_configs[RCC_CLOCK_3V3_96MHZ] provided by libopencm3 g4 code. */
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_96MHZ]);

    /* Optional: check system frequency vars (debug) */
    /* rcc_ahb_frequency should now be 96_000_000 */

    gpio_setup();

    while (1) {
        gpio_toggle(LED_PORT, LED_PIN);
        delay_ms_approx(5000);
    }

    /* never reached */
    return 0;
}
