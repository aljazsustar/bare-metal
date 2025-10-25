/* main.c — for NUCLEO-G491RE (STM32G491RE) with libopencm3 built for stm32/g4 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/cm3/nvic.h>
#include "core/system.h"
#include "core/timer.h"

#define LED_PORT GPIOA
#define LED_PIN GPIO5

#define BTN_PORT GPIOC
#define BTN_PIN GPIO13

#define BRIGHTEN 1
#define DIMM 0

#define DUTY_CYCLE_STEP 0.5f

static void gpio_setup(void)
{
    /* enable GPIOA clock and set PA5 as output */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_SYSCFG);

    gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);
    gpio_set_af(LED_PORT, GPIO_AF1, LED_PIN);

    gpio_mode_setup(BTN_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BTN_PIN);
}

static void exti_setup(void)
{
    exti_select_source(EXTI13, BTN_PORT);
    exti_set_trigger(EXTI13, EXTI_TRIGGER_FALLING);
    exti_enable_request(EXTI13);

    nvic_enable_irq(NVIC_EXTI15_10_IRQ);
}

void exti15_10_isr(void)
{
    if (exti_get_flag_status(EXTI13))
    {
        gpio_toggle(LED_PORT, LED_PIN); // Toggle LED

        /* Clear interrupt flag */
        exti_reset_request(EXTI13);
    }
}

int main(void)
{
    system_setup();
    gpio_setup();
    timer_setup();
    exti_setup();

    uint64_t start_time = system_get_ticks();
    float duty_cycle = 0.0f;
    uint8_t direction = BRIGHTEN;

    while (1)
    {
        if (system_get_ticks() - start_time >= 10)
        {

            if (duty_cycle > 100.0f)
                direction = DIMM;
            else if (duty_cycle < 0.0f)
                direction = BRIGHTEN;

            if (direction == 1)
                duty_cycle += DUTY_CYCLE_STEP;
            else
                duty_cycle -= DUTY_CYCLE_STEP;

            timer_pwm_set_duty_cycle(duty_cycle);
            start_time = system_get_ticks();
        }
    }

    while (1)
    {
        __asm__("wfi");
    }
    /* never reached */
    return 0;
}
