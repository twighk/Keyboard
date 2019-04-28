#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	struct HidKeyboard {
		bool left_control : 1;
		bool left_shift : 1;
		bool left_alt : 1;
		bool left_gui : 1;
		bool right_control : 1;
		bool right_shift : 1;
		bool right_alt : 1;
		bool right_gui : 1;
		uint8_t reserved;
		uint8_t keys[6];
	} __attribute__((packed));

	struct HidKeyboard ProcessKeys(struct HidKeyboard * hidkbd);

	void InitKeyboard();

	void SetGLed(uint8_t l);

#ifdef __cplusplus
}
#endif