/*
 * KeyboardFirmwareMain.cpp
 *
 *  Created on: 4 Nov 2017
 *      Author: twig
 */


#include "KeyboardFirmware.h"

#include "LEDs.h"


extern "C" {

struct Hardware hardware;

void KFMain(){

	InitLeds();

	while(true){
		SetBLed(0xFFFF);
		SetRLed(0x0);

		HAL_Delay(666);

		SetRLed(0xFFFF);
		SetGLed(0x0);

		HAL_Delay(666);

		SetGLed(0xFFFF);
		SetBLed(0x0);

		HAL_Delay(666);
	}

}


} // End extern "C"

