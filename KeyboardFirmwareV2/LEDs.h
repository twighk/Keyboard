#pragma once

#include <cstdint>

#include "nrfx_pwm.h"
#include "nrf_gpio.h"

class LEDs {
	const uint8_t R_PIN = NRF_GPIO_PIN_MAP(0,20) | NRFX_PWM_PIN_INVERTED;
	const uint8_t G_PIN = NRF_GPIO_PIN_MAP(0,21) | NRFX_PWM_PIN_INVERTED;
	const uint8_t B_PIN = NRF_GPIO_PIN_MAP(0,22) | NRFX_PWM_PIN_INVERTED;

	nrfx_pwm_t const pwm;
 	nrfx_pwm_config_t const config;
	nrf_pwm_values_individual_t seq_values;
	nrf_pwm_sequence_t const seq;

public:

	LEDs()
		: pwm (NRFX_PWM_INSTANCE(0))
		, config 
			{ .output_pins = 
				{ R_PIN 
				, G_PIN
				, B_PIN
				, NRFX_PWM_PIN_NOT_USED
				}
			, .irq_priority = APP_IRQ_PRIORITY_LOWEST
			, .base_clock = NRF_PWM_CLK_16MHz
			, .count_mode = NRF_PWM_MODE_UP
			, .top_value = 0xFFFF
			, .load_mode = NRF_PWM_LOAD_INDIVIDUAL
			, .step_mode = NRF_PWM_STEP_AUTO
			}
		, seq_values 
			{ .channel_0 = 0
			, .channel_1 = 0
			, .channel_2 = 0
			, .channel_3 = 0
			}
		, seq 
			{ .values = {.p_individual = &seq_values}
			, .length = NRF_PWM_VALUES_LENGTH(seq_values)
			, .repeats = 0
			, .end_delay = 0
			}
	{
		nrfx_pwm_init(&pwm, &config, NULL);
		nrfx_pwm_simple_playback(&pwm, &seq, 1, NRFX_PWM_FLAG_LOOP);
	}

	void setR(int16_t brightness){
		seq_values.channel_0 = brightness;
	}
	void setG(int16_t brightness){
		seq_values.channel_1 = brightness;
	}
	void setB(int16_t brightness){
		seq_values.channel_2 = brightness;
	}

	void setAllOff(){
		setR(0);
		setG(0);
		setB(0);
	}
};
