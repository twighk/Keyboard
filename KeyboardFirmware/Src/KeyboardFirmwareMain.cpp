/*
 * KeyboardFirmwareMain.cpp
 *
 *  Created on: 4 Nov 2017
 *      Author: twig
 */


#include "KeyboardFirmware.h"
#include "usb_device.h"
#include "usbd_hid.h"

#include "LEDs.h"
#include "Keycodes.h"

void Blinky();
void ManualDisco();

template<typename T>
uint8_t UsbSend(T * const s){
	return USBD_HID_SendReport(&hUsbDeviceFS, reinterpret_cast<uint8_t * const>(s), sizeof(T));
}

// Hid Keyboard Packet
struct HidKeyboard{
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
	
	HidKeyboard(){
		left_control = false;
		left_shift = false;
		left_alt = false;
		left_gui = false;
		right_control = false;
		right_shift = false;
		right_alt = false;
		right_gui = false;
		reserved = 0;
		for (int i = 0; i != sizeof(keys)/sizeof(keys[0]); ++i)
			keys[i] = 0;
	}
} __attribute__((packed));
static_assert(sizeof(HidKeyboard) == 8 , "Sizeof HidKeyboard is not 8");

//Bit field of the pressed keys
struct KeyBits {

	uint32_t keys[5];
	
	KeyBits(){
		for(int c = 0; c != sizeof(keys) / sizeof(*keys); ++c){
			keys[c] = 0;
		}
	}

	void Read(){
		auto readkeys = (IsRightBoard()? ReadRhsKeys : ReadLhsKeys);

		HAL_GPIO_WritePin(Row0_GPIO_Port, Row0_Pin, GPIO_PIN_SET);
		keys[0] = readkeys();
		HAL_GPIO_WritePin(Row0_GPIO_Port, Row0_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(Row1_GPIO_Port, Row1_Pin, GPIO_PIN_SET);
		keys[1] = readkeys();
		HAL_GPIO_WritePin(Row1_GPIO_Port, Row1_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(Row2_GPIO_Port, Row2_Pin, GPIO_PIN_SET);
		keys[2] = readkeys();
		HAL_GPIO_WritePin(Row2_GPIO_Port, Row2_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(Row3_GPIO_Port, Row3_Pin, GPIO_PIN_SET);
		keys[3] = readkeys();
		HAL_GPIO_WritePin(Row3_GPIO_Port, Row3_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(Row4_GPIO_Port, Row4_Pin, GPIO_PIN_SET);
		keys[4] = readkeys();
		HAL_GPIO_WritePin(Row4_GPIO_Port, Row4_Pin, GPIO_PIN_RESET);
	}

	static bool IsRightBoard(){
		return GPIO_PIN_SET == HAL_GPIO_ReadPin(LorR_GPIO_Port, LorR_Pin);
	}

	static uint32_t ReadLhsKeys(){
		return 0
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col0_GPIO_Port, Col0_Pin)?1:0) << 0
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col1_GPIO_Port, Col1_Pin)?1:0) << 1
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col2_GPIO_Port, Col2_Pin)?1:0) << 2
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col3_GPIO_Port, Col3_Pin)?1:0) << 3
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col4_GPIO_Port, Col4_Pin)?1:0) << 4
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col5_GPIO_Port, Col5_Pin)?1:0) << 5
		;
	}

	static uint32_t ReadRhsKeys(){
		return 0
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin( Col6_GPIO_Port, Col6_Pin)?1:0)  <<  6
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin( Col7_GPIO_Port, Col7_Pin)?1:0)  <<  7
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin( Col8_GPIO_Port, Col8_Pin)?1:0)  <<  8
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin( Col9_GPIO_Port, Col9_Pin)?1:0)  <<  9
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col10_GPIO_Port, Col10_Pin)?1:0) << 10
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col11_GPIO_Port, Col11_Pin)?1:0) << 11
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col12_GPIO_Port, Col12_Pin)?1:0) << 12
		| (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col13_GPIO_Port, Col13_Pin)?1:0) << 13
		;
	}

	const KeyBits operator^(const KeyBits & k){
		KeyBits out;

		for(int c = 0; c != sizeof(keys) / sizeof(*keys); ++c){
			out.keys[c] = keys[c] ^ k.keys[c];	
		}
		return out;
	}
};

void nil(const KeyBits &, bool, HidKeyboard&){}

void lshift(const KeyBits &, bool pressed, HidKeyboard& hk){
	hk.left_shift = pressed;
}
void rshift(const KeyBits &, bool pressed, HidKeyboard& hk){
	hk.right_shift = pressed;
}

void lctrl(const KeyBits &, bool pressed, HidKeyboard& hk){
	hk.left_control = pressed;
}

void rctrl(const KeyBits &, bool pressed, HidKeyboard& hk){
	hk.right_control = pressed;
}

void lalt(const KeyBits &, bool pressed, HidKeyboard& hk){
	hk.left_alt = pressed;
}

void ralt(const KeyBits &, bool pressed, HidKeyboard& hk){
	hk.right_alt = pressed;
}

void lgui(const KeyBits &, bool pressed, HidKeyboard& hk){
	hk.left_gui = pressed;
}

void rgui(const KeyBits &, bool pressed, HidKeyboard& hk){
	hk.right_gui = pressed;
}
template <uint8_t keycode>
void k(const KeyBits &, bool pressed, HidKeyboard& hk){
	if(pressed){
		for (int i = 0; i != sizeof(hk.keys)/sizeof(*hk.keys); ++i){
			if(hk.keys[i] == keycode)
				return;
		}
	}

	for (int i = 0; i != sizeof(hk.keys)/sizeof(*hk.keys); ++i){
		if(pressed){
			if(hk.keys[i] == 0){
				hk.keys[i] = keycode;
				return;
			}
		} else {
			if(hk.keys[i] == keycode)
				hk.keys[i] = 0;
		}
	}
}

bool SpecialKeyPressed(const KeyBits &kb){
	return kb.keys[4] & (1<<1 | 1<<12);
}

template <uint8_t normalkeycode, uint8_t specialkeycode>
void s(const KeyBits &kb, bool pressed, HidKeyboard& hk){
	const uint8_t keycode = (!SpecialKeyPressed(kb) ? normalkeycode : specialkeycode);
	if(pressed){
		for (int i = 0; i != sizeof(hk.keys)/sizeof(*hk.keys); ++i){
			if(hk.keys[i] == keycode)
				return;
		}
	}

	for (int i = 0; i != sizeof(hk.keys)/sizeof(*hk.keys); ++i){
		if(pressed){
			if(hk.keys[i] == 0){
				hk.keys[i] = keycode;
				return;
			}
		} else {
			if(hk.keys[i] == normalkeycode || hk.keys[i] == specialkeycode)
				hk.keys[i] = 0;
		}
	}
}

uint16_t led_brightnesses[] = 
	{ 0xFFFF
	, 0x03E0
	, 0x03C0
	, 0x0380
	, 0x0300
	, 0x0100
	, 0x0000
	};

void bled(const KeyBits&, bool pressed, HidKeyboard&){
	if (pressed){
		SetBLed(led_brightnesses[2]);
	} else {
		SetBLed(0xFFFF);
	}
}

void (*KeyCodes[5][14])(const KeyBits &, bool pressed, HidKeyboard & hk) = 
	{{s<K_grave,K_esc>,s<K_1,K_f1>,s<K_2,K_f2>,s<K_3,K_f3>,s<K_4,K_f4>,s<K_5,K_f5>
		 ,s<K_6,K_f6>,s<K_7,K_f7>,s<K_8,K_f8>,s<K_9,K_f9>,s<K_0,K_f10>,s<K_minus,K_f11>,s<K_equal,K_f12>,s<K_bks,K_del>}		
	,{k<K_tab>,k<K_q>,k<K_w>,k<K_e>,k<K_r>,k<K_t>
		 ,k<K_y>,k<K_u>,s<K_i,K_end>,s<K_o,K_pgdown>,s<K_p,K_up>,s<K_sqopen,K_pgup>,s<K_sqclose,K_home>,k<K_bslash>}		
	,{k<K_esc>,k<K_a>,k<K_s>,k<K_d>,k<K_f>,k<K_g>
		 ,k<K_h>,k<K_j>,k<K_k>,s<K_l,K_left>,s<K_scolon,K_down>,s<K_quote,K_right>,nil,k<K_ret>}		
	,{lshift,s<K_z,K_mute>,s<K_x,K_vol_down>,s<K_c,K_vol_up>,k<K_v>,k<K_b>
		 ,k<K_n>,k<K_m>,k<K_comma>,k<K_fstop>,k<K_fslash>,nil,nil,rshift}		
	,{lctrl,bled,lgui,lalt,s<K_space,K_del>,nil
		 ,s<K_space,K_del>,nil,nil,ralt,rgui,k<K_context>,bled,rctrl}
	};

static KeyBits previous_kbs = KeyBits();

const HidKeyboard ProcessKeys(HidKeyboard hk){
	KeyBits kbs;
	kbs.Read();
	KeyBits delta = previous_kbs ^ kbs;

	for (size_t c = 0; c < 5 ; c += 1){
		for (size_t r = 0; r < 14; r += 1){
			if (delta.keys[c] & 1 << r){
				KeyCodes[c][r](kbs, kbs.keys[c] & 1 << r, hk);
			} 	
		} 
	}

	previous_kbs = kbs;
	return hk;
}

extern "C" {

struct Hardware hardware;


void KFMain(){

	MX_USB_DEVICE_Init();
	InitLeds();

	struct HidKeyboard hidkeyboard_last;
	while(true){

		HidKeyboard hidkeyboard = ProcessKeys(hidkeyboard_last);

		if (memcmp(&hidkeyboard_last, &hidkeyboard, sizeof(hidkeyboard)) != 0){

			UsbSend(&hidkeyboard);

			USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef*)hUsbDeviceFS.pClassData;
			HAL_Delay(2);
			if(hhid->state != HID_IDLE){
				SetRLed(0x0000);
			} else {
				SetRLed(0xFFFF);
			}
		}

		hidkeyboard_last = hidkeyboard;
		
		int num_keys_set = 0;
		for(int i = 0; i != sizeof(hidkeyboard.keys)/sizeof(*hidkeyboard.keys); ++i){
			if(hidkeyboard.keys[i] != 0){
				num_keys_set += 1;
			}
		}
			
		SetGLed(led_brightnesses[num_keys_set]);
	}

}


} // End extern "C"

