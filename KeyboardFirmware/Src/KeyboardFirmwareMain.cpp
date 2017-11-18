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

struct HidMouse {
	uint8_t buttons;
	int8_t x;
	int8_t y;
	int8_t wheel;
};


void KFMain(){

	MX_USB_DEVICE_Init();
	InitLeds();

	//ManualDisco();

	struct HidMouse hidmouse;

	hidmouse.buttons = 0;
	hidmouse.x = 10;
	hidmouse.y = 10;
	hidmouse.wheel = 0;

	while(true){

		HAL_GPIO_WritePin(Row1_GPIO_Port, Row1_Pin, GPIO_PIN_SET);
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Col11_GPIO_Port, Col11_Pin)){
			UsbSend(&hidmouse);
			HAL_Delay(1000);
		}

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
