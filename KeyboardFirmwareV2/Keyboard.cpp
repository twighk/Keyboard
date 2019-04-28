#include "Keyboard.h"

#include "KeyBits.h"
#include "Keycodes.h"
#include "LEDs.h"

const HidKeyboard ProcessKeysCpp(HidKeyboard hk);

LEDs leds;

uint16_t led_brightnesses[] =
{ 0xFFFF
, 0x03E0
, 0x03C0
, 0x0380
, 0x0300
, 0x0100
, 0x0000
};

extern "C" {
	struct HidKeyboard ProcessKeys(struct HidKeyboard * hidkbd) {
		return ProcessKeysCpp(*hidkbd);
	}

	void InitKeyboard() {
		KeyBits::InitKeyboard();
		leds.setAllOff();
	}

	void SetGLed(uint8_t l) {
		leds.setG(0xFFFF - led_brightnesses[l]);
	}
}


void nil(const KeyBits &, bool, HidKeyboard&) {}

void lshift(const KeyBits &, bool pressed, HidKeyboard& hk) {
	hk.left_shift = pressed;
}
void rshift(const KeyBits &, bool pressed, HidKeyboard& hk) {
	hk.right_shift = pressed;
}

void lctrl(const KeyBits &, bool pressed, HidKeyboard& hk) {
	hk.left_control = pressed;
}

void rctrl(const KeyBits &, bool pressed, HidKeyboard& hk) {
	hk.right_control = pressed;
}

void lalt(const KeyBits &, bool pressed, HidKeyboard& hk) {
	hk.left_alt = pressed;
}

void ralt(const KeyBits &, bool pressed, HidKeyboard& hk) {
	hk.right_alt = pressed;
}

void lgui(const KeyBits &, bool pressed, HidKeyboard& hk) {
	hk.left_gui = pressed;
}

void rgui(const KeyBits &, bool pressed, HidKeyboard& hk) {
	hk.right_gui = pressed;
}

template <uint8_t keycode>
void k(const KeyBits &, bool pressed, HidKeyboard& hk) {
	if (pressed) {
		for (int i = 0; i != sizeof(hk.keys) / sizeof(*hk.keys); ++i) {
			if (hk.keys[i] == keycode)
				return;
		}
	}

	for (int i = 0; i != sizeof(hk.keys) / sizeof(*hk.keys); ++i) {
		if (pressed) {
			if (hk.keys[i] == 0) {
				hk.keys[i] = keycode;
				return;
			}
		}
		else {
			if (hk.keys[i] == keycode)
				hk.keys[i] = 0;
		}
	}
}

bool SpecialKeyPressed(const KeyBits &kb) {
	return kb.keys[4] & (1 << 1 | 1 << 12);
}

template <uint8_t normalkeycode, uint8_t specialkeycode>
void s(const KeyBits &kb, bool pressed, HidKeyboard& hk) {
	const uint8_t keycode = (!SpecialKeyPressed(kb) ? normalkeycode : specialkeycode);
	if (pressed) {
		for (int i = 0; i != sizeof(hk.keys) / sizeof(*hk.keys); ++i) {
			if (hk.keys[i] == keycode)
				return;
		}
	}

	for (int i = 0; i != sizeof(hk.keys) / sizeof(*hk.keys); ++i) {
		if (pressed) {
			if (hk.keys[i] == 0) {
				hk.keys[i] = keycode;
				return;
			}
		}
		else {
			if (hk.keys[i] == normalkeycode || hk.keys[i] == specialkeycode)
				hk.keys[i] = 0;
		}
	}
}


void bled(const KeyBits&, bool pressed, HidKeyboard&) {
	if (pressed) {
		//SetBLed(led_brightnesses[2]);
	}
	else {
		//SetBLed(0xFFFF);
	}
}

void(*KeyCodes[5][14])(const KeyBits &, bool pressed, HidKeyboard & hk) =
{ { s<K_grave,K_esc>,s<K_1,K_f1>,s<K_2,K_f2>,s<K_3,K_f3>,s<K_4,K_f4>,s<K_5,K_f5>
	,s<K_6,K_f6>,s<K_7,K_f7>,s<K_8,K_f8>,s<K_9,K_f9>,s<K_0,K_f10>,s<K_minus,K_f11>,s<K_equal,K_f12>,s<K_bks,K_del> }
,{ k<K_tab>,k<K_q>,k<K_w>,k<K_e>,k<K_r>,k<K_t>
	,k<K_y>,k<K_u>,s<K_i,K_end>,s<K_o,K_pgdown>,s<K_p,K_up>,s<K_sqopen,K_pgup>,s<K_sqclose,K_home>,k<K_bslash> }
,{ k<K_esc>,k<K_a>,k<K_s>,k<K_d>,k<K_f>,k<K_g>
	,k<K_h>,k<K_j>,k<K_k>,s<K_l,K_left>,s<K_scolon,K_down>,s<K_quote,K_right>,nil,k<K_ret> }
,{ lshift,s<K_z,K_mute>,s<K_x,K_vol_down>,s<K_c,K_vol_up>,k<K_v>,k<K_b>
	,k<K_n>,k<K_m>,k<K_comma>,k<K_fstop>,k<K_fslash>,nil,nil,rshift }
,{ lctrl,bled,lgui,lalt,s<K_space,K_del>,nil
	,s<K_space,K_del>,nil,nil,ralt,rgui,k<K_context>,bled,rctrl }
};



static KeyBits previous_kbs = KeyBits();

const HidKeyboard ProcessKeysCpp(HidKeyboard hk) {
	KeyBits kbs;
	kbs.Read();
	KeyBits delta = previous_kbs ^ kbs;

	for (size_t c = 0; c < 5; c += 1) {
		for (size_t r = 0; r < 14; r += 1) {
			if (delta.keys[c] & 1 << r) {
				KeyCodes[c][r](kbs, kbs.keys[c] & 1 << r, hk);
			}
		}
	}

	previous_kbs = kbs;
	return hk;
}
