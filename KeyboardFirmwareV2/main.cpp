#include <stdio.h>
#include <string.h>
#include "ble.h"
#include "ble_hids.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"
#include "nrf_gpio.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"

#include "LEDs.h"
#include "KeyBits.h"



int main(void)
{
	LEDs leds;
	KeyBits kbs;
	

    while (true)
    {	
		kbs.Read();
		leds.setR((kbs.Any()) != 0 ? 0x8000 : 0);
		leds.setG((kbs.keys[2] & (1 << 2)) != 0 ? 0x8000 : 0);
		leds.setB((kbs.keys[2] & (1 << 3)) != 0 ? 0x8000 : 0);
		nrf_delay_ms(10);
    }
}

