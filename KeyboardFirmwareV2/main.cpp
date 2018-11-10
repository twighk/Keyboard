#include <stdio.h>
#include <string.h>
#include "nrf_delay.h"
#include "nrfx_pwm.h"
#include "nrf_gpio.h"

#include "LEDs.h"


/**
 *  * @brief Function for application main entry.
 *   */
int main(void)
{
	/* Configure board. */
	//nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0,20));
	//nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0,21));
	//nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0,22));
    /* Toggle LEDs. */
	LEDs leds;
    while (true)
    {	
		for(uint16_t i = 0; i != 0xFFFF; ++i){
			leds.setG(i);
			nrf_delay_ms(1);
		}
		//for(uint16_t i = 0xFFFF; i != 0x0; --i){
		//	leds.setG(i);
		//	nrf_delay_ms(1);
		//}
		//seq_values.channel_0 = (seq_values.channel_0 + 5) % 9000;
		//seq_values.channel_1 = (seq_values.channel_1 + 5) % 9000;
		//seq_values.channel_2 = (seq_values.channel_2 + 5) % 9000;
		//nrf_gpio_pin_toggle(NRF_GPIO_PIN_MAP(0,20));
		//nrf_gpio_pin_toggle(NRF_GPIO_PIN_MAP(0,21));
		//nrf_gpio_pin_toggle(NRF_GPIO_PIN_MAP(0,22));
    }
}
