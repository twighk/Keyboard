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
		for(int c = 0; c != sizeof(keys) / sizeof(keys[0]); ++c){
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
			if(hk.keys[i] == 0)
				hk.keys[i] = keycode;
			return;
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
			if(hk.keys[i] == 0)
				hk.keys[i] = keycode;
			return;
		} else {
			if(hk.keys[i] == normalkeycode || hk.keys[i] == specialkeycode)
				hk.keys[i] = 0;
		}
	}
}

const uint8_t K_a = 0x04;
const uint8_t K_b = 0x05;
const uint8_t K_c = 0x06;
const uint8_t K_d = 0x07;
const uint8_t K_e = 0x08;
const uint8_t K_f = 0x09;
const uint8_t K_g = 0x0a;
const uint8_t K_h = 0x0b;
const uint8_t K_i = 0x0c;
const uint8_t K_j = 0x0d;
const uint8_t K_k = 0x0e;
const uint8_t K_l = 0x0f;
const uint8_t K_m = 0x10;
const uint8_t K_n = 0x11;
const uint8_t K_o = 0x12;
const uint8_t K_p = 0x13;
const uint8_t K_q = 0x14;
const uint8_t K_r = 0x15;
const uint8_t K_s = 0x16;
const uint8_t K_t = 0x17;
const uint8_t K_u = 0x18;
const uint8_t K_v = 0x19;
const uint8_t K_w = 0x1a;
const uint8_t K_x = 0x1b;
const uint8_t K_y = 0x1c;
const uint8_t K_z = 0x1d;

const uint8_t K_1 = 0x1e;
const uint8_t K_2 = 0x1f;
const uint8_t K_3 = 0x20;
const uint8_t K_4 = 0x21;
const uint8_t K_5 = 0x22;
const uint8_t K_6 = 0x23;
const uint8_t K_7 = 0x24;
const uint8_t K_8 = 0x25;
const uint8_t K_9 = 0x26;
const uint8_t K_0 = 0x27;

const uint8_t K_ret = 0x28;
const uint8_t K_esc = 0x29;
const uint8_t K_bks = 0x2a;
const uint8_t K_tab = 0x2b;
const uint8_t K_space = 0x2c;

const uint8_t K_minus = 0x2d;
const uint8_t K_equal = 0x2e;

const uint8_t K_sqopen = 0x2f;
const uint8_t K_sqclose = 0x30;
const uint8_t K_bslash = 0x31;

const uint8_t K_scolon = 0x33;
const uint8_t K_quote = 0x34;

const uint8_t K_grave = 0x34;

const uint8_t K_comma = 0x36;
const uint8_t K_fstop = 0x37;
const uint8_t K_fslash = 0x38;

const uint8_t K_clock = 0x39;

const uint8_t K_f1 = 0x3a;
const uint8_t K_f2 = 0x3b;
const uint8_t K_f3 = 0x3c;
const uint8_t K_f4 = 0x3d;
const uint8_t K_f5 = 0x3e;
const uint8_t K_f6 = 0x3f;
const uint8_t K_f7 = 0x40;
const uint8_t K_f8 = 0x41;
const uint8_t K_f9 = 0x42;
const uint8_t K_f10 = 0x43;
const uint8_t K_f11 = 0x44;
const uint8_t K_f12 = 0x45;

const uint8_t K_right = 0x4f;
const uint8_t K_left = 0x50;
const uint8_t K_down = 0x51;
const uint8_t K_up = 0x52;


void (*KeyCodes[5][14])(const KeyBits &, bool pressed, HidKeyboard & hk) = 
	{{k<K_grave>,k<K_1>,k<K_2>,k<K_3>,k<K_4>,k<K_5>
		 ,s<K_6,K_f6>,s<K_7,K_f7>,s<K_8,K_f8>,s<K_9,K_f9>,s<K_0,K_f10>,s<K_minus,K_f11>,s<K_equal,K_f12>,k<K_bks>}		
	,{k<K_tab>,k<K_q>,k<K_w>,k<K_e>,k<K_r>,k<K_t>
		 ,k<K_y>,k<K_u>,k<K_i>,k<K_o>,s<K_p,K_up>,k<K_sqopen>,k<K_sqclose>,k<K_bslash>}		
	,{k<K_esc>,k<K_a>,k<K_s>,k<K_d>,k<K_f>,k<K_g>
		 ,k<K_h>,k<K_j>,k<K_k>,s<K_l,K_left>,s<K_scolon,K_down>,s<K_quote,K_right>,nil,k<K_ret>}		
	,{lshift,k<K_z>,k<K_x>,k<K_c>,k<K_v>,k<K_b>
		 ,k<K_n>,k<K_m>,k<K_comma>,k<K_fstop>,k<K_fslash>,nil,nil,rshift}		
	,{lctrl,k<0>,lgui,lalt,k<K_space>,nil
		 ,k<K_space>,nil,nil,ralt,rgui,k<0>,k<0>,rctrl}
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

	//ManualDisco();

	HAL_GPIO_WritePin(BM_EN_GPIO_Port,BM_EN_Pin,GPIO_PIN_RESET); // 
	HAL_GPIO_WritePin(BMRES_GPIO_Port,BMRES_Pin,GPIO_PIN_RESET); // 
	HAL_Delay(66);
	HAL_GPIO_WritePin(BMRES_GPIO_Port,BMRES_Pin,GPIO_PIN_SET); // 

	struct HidKeyboard hidkeyboard_last;
	while(true){

		HidKeyboard hidkeyboard = ProcessKeys(hidkeyboard_last);

		if (memcmp(&hidkeyboard_last, &hidkeyboard, sizeof(hidkeyboard)) != 0){
			//HAL_UART_Transmit_IT(hardware.huart1, &hidkeyboard.keys[i], 1);
			UsbSend(&hidkeyboard);
			SetBLed(0xFFFF);
			HAL_Delay(66);
			SetBLed(0x200);
		}

		hidkeyboard_last = hidkeyboard;
	}

}


} // End extern "C"



void Blinky(){
	while(true){
		SetBLed(0xFFFF);
		SetRLed(0x200);

		HAL_Delay(666);

		SetRLed(0xFFFF);
		SetGLed(0x200);

		HAL_Delay(666);

		SetGLed(0xFFFF);
		SetBLed(0x200);

		HAL_Delay(666);
	}
}

void ManualDisco(){
	while(true){
		KeyBits kbs = KeyBits();
		kbs.Read();

		if (kbs.keys[1] & 1 << 12){
			SetBLed(0x200);
		} else {
			SetBLed(0xFFFF);
		}

		if (kbs.keys[1] & 1 << 13){
			SetRLed(0x300);
		} else {
			SetRLed(0xFFFF);
		}

		if (kbs.keys[1] & 1 << 11){
			SetGLed(0x300);
		} else {
			SetGLed(0xFFFF);
		}

	}
}
