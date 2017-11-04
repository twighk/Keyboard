/*
 * KeyboardFirmwareMain.cpp
 *
 *  Created on: 4 Nov 2017
 *      Author: twig
 */


#include "KeyboardFirmware.h"

#include "LEDs.h"

void Blinky();
void ManualDisco();

extern "C" {

struct Hardware hardware;

void KFMain(){

	InitLeds();

	ManualDisco();

	while(true){

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
