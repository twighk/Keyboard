#pragma once

#include <cstdint>

#include "nrf_gpio.h"
#include "app_pwm.h"


class LEDs {
	const uint8_t R_PIN = NRF_GPIO_PIN_MAP(0,20);
	//const uint8_t G_PIN = NRF_GPIO_PIN_MAP(0,21) | NRFX_PWM_PIN_INVERTED;
	const uint8_t B_PIN = NRF_GPIO_PIN_MAP(0,22);

public:

	LEDs()
	{

	}

	void setR(int16_t brightness){
	}
	void setG(int16_t brightness){
	}
	void setB(int16_t brightness){
	}

	void setAllOff(){
		setR(0);
		setG(0);
		setB(0);
	}
};
