#pragma once

#include "nrf_gpio.h"

struct KeyBits {
    static const uint8_t LorR = NRF_GPIO_PIN_MAP(0,3);

    static const uint8_t Col0 = NRF_GPIO_PIN_MAP(1,0);
    static const uint8_t Col1 = NRF_GPIO_PIN_MAP(1,1);
    static const uint8_t Col2 = NRF_GPIO_PIN_MAP(1,2);
    static const uint8_t Col3 = NRF_GPIO_PIN_MAP(1,3);
    static const uint8_t Col4 = NRF_GPIO_PIN_MAP(1,4);
    static const uint8_t Col5 = NRF_GPIO_PIN_MAP(1,5);
    static const uint8_t Col6 = NRF_GPIO_PIN_MAP(1,6);
    static const uint8_t Col7 = NRF_GPIO_PIN_MAP(1,7);

    static const uint8_t Row0 = NRF_GPIO_PIN_MAP(1,10);
    static const uint8_t Row1 = NRF_GPIO_PIN_MAP(1,11);
    static const uint8_t Row2 = NRF_GPIO_PIN_MAP(1,12);
    static const uint8_t Row3 = NRF_GPIO_PIN_MAP(1,13);
    static const uint8_t Row4 = NRF_GPIO_PIN_MAP(1,14);

	uint32_t keys[5];
	
	KeyBits(){
		for(int c = 0; c != sizeof(keys) / sizeof(*keys); ++c){
			keys[c] = 0;
		}

		InitKeyboard();
	}
	
	static void InitKeyboard(){
		static bool inited = false;

		if (inited)
			return;
		inited = true;

		// Left or Right
		nrf_gpio_cfg_input(LorR,NRF_GPIO_PIN_NOPULL);

		// Columns for reading
		nrf_gpio_range_cfg_input(Col0,Col7,NRF_GPIO_PIN_PULLDOWN);

		// Rows for writing
		nrf_gpio_range_cfg_output(Row0,Row4);
	}

	void Read(){
		uint32_t (* const readkeys)(void) = (IsRightBoard()? ReadRhsKeys : ReadLhsKeys);

		while(readkeys() != 0);

		nrf_gpio_pin_set(Row0);
		keys[0] = readkeys();
		nrf_gpio_pin_clear(Row0);

		while(readkeys() != 0);

		nrf_gpio_pin_set(Row1);
		keys[1] = readkeys();
		nrf_gpio_pin_clear(Row1);

		while(readkeys() != 0);

		nrf_gpio_pin_set(Row2);
		keys[2] = readkeys();
		nrf_gpio_pin_clear(Row2);
		
		while(readkeys() != 0);

		nrf_gpio_pin_set(Row3);
		keys[3] = readkeys();
		nrf_gpio_pin_clear(Row3);
		
		while(readkeys() != 0);

		nrf_gpio_pin_set(Row4);
		keys[4] = readkeys();
		nrf_gpio_pin_clear(Row4);
		}

	static bool IsRightBoard(){
		return nrf_gpio_pin_read(LorR);
	}

	static uint32_t ReadLhsKeys(){
		return 0
			| (nrf_gpio_pin_read(Col0) ? 1 <<  0 : 0) 
			| (nrf_gpio_pin_read(Col1) ? 1 <<  1 : 0) 
			| (nrf_gpio_pin_read(Col2) ? 1 <<  2 : 0) 
			| (nrf_gpio_pin_read(Col3) ? 1 <<  3 : 0) 
			| (nrf_gpio_pin_read(Col4) ? 1 <<  4 : 0) 
			| (nrf_gpio_pin_read(Col5) ? 1 <<  5 : 0) 
			;
	}

	static uint32_t ReadRhsKeys(){
		return 0
			| (nrf_gpio_pin_read(Col0) ? 1 <<  6 : 0)
			| (nrf_gpio_pin_read(Col1) ? 1 <<  7 : 0)
			| (nrf_gpio_pin_read(Col2) ? 1 <<  8 : 0)
			| (nrf_gpio_pin_read(Col3) ? 1 <<  9 : 0)
			| (nrf_gpio_pin_read(Col4) ? 1 << 10 : 0)
			| (nrf_gpio_pin_read(Col5) ? 1 << 11 : 0)
			| (nrf_gpio_pin_read(Col6) ? 1 << 12 : 0)
			| (nrf_gpio_pin_read(Col7) ? 1 << 13 : 0)
			;
	}

	const KeyBits operator^(const KeyBits & k){
		KeyBits out;

		for(int c = 0; c != sizeof(keys) / sizeof(*keys); ++c){
			out.keys[c] = keys[c] ^ k.keys[c];	
		}

		return out;
	}

	const bool Any() const {
		return keys[0] | keys[1] | keys[2] | keys[3] | keys[4];
	}
};
