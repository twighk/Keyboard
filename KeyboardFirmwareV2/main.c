#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

/**
 *  * @brief Function for application main entry.
 *   */
int main(void)
{
	/* Configure board. */
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0,20));
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0,21));
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0,22));
    /* Toggle LEDs. */
    while (true)
    {
		nrf_gpio_pin_toggle(NRF_GPIO_PIN_MAP(0,20));
		nrf_delay_ms(500);
		nrf_gpio_pin_toggle(NRF_GPIO_PIN_MAP(0,21));
		nrf_delay_ms(500);
		nrf_gpio_pin_toggle(NRF_GPIO_PIN_MAP(0,22));
		nrf_delay_ms(500);
    }
}
