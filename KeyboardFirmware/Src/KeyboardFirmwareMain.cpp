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

extern "C" {

struct Hardware hardware;

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
		struct HidKeyboard hidkeyboard;

		HAL_GPIO_WritePin(Row1_GPIO_Port, Row1_Pin, GPIO_PIN_SET);
		hidkeyboard.keys[0] = (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col11_GPIO_Port, Col11_Pin)? 4 : 0);
		hidkeyboard.keys[1] = (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col10_GPIO_Port, Col10_Pin)? 5 : 0);
		HAL_GPIO_WritePin(Row1_GPIO_Port, Row1_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(Row2_GPIO_Port, Row2_Pin, GPIO_PIN_SET);
		hidkeyboard.keys[2] = (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col11_GPIO_Port, Col11_Pin)? 6 : 0);
		hidkeyboard.keys[3] = (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col10_GPIO_Port, Col10_Pin)? 7 : 0);
		HAL_GPIO_WritePin(Row2_GPIO_Port, Row2_Pin, GPIO_PIN_RESET);

		if (memcmp(&hidkeyboard_last, &hidkeyboard, sizeof(hidkeyboard))){
			int i;
			for (i = 0; i != 3; ++i)
				if (hidkeyboard.keys[i] != 0)
					break;
			HAL_UART_Transmit_IT(hardware.huart1, &hidkeyboard.keys[i], 1);
			//UsbSend(&hidkeyboard);
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
		HAL_GPIO_WritePin(Row1_GPIO_Port, Row1_Pin, GPIO_PIN_SET);
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col11_GPIO_Port, Col11_Pin)){
			SetBLed(0x200);
		} else {
			SetBLed(0xFFFF);
		}

		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col10_GPIO_Port, Col10_Pin)){
			SetRLed(0x300);
		} else {
			SetRLed(0xFFFF);
		}
		HAL_GPIO_WritePin(Row1_GPIO_Port, Row1_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(Row2_GPIO_Port, Row2_Pin, GPIO_PIN_SET);
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col11_GPIO_Port, Col11_Pin)){
			SetGLed(0x300);
		} else {
			SetGLed(0xFFFF);
		}
		HAL_GPIO_WritePin(Row2_GPIO_Port, Row2_Pin, GPIO_PIN_RESET);

	}
}
