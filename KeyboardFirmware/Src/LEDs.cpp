/*
 * LEDs.cpp
 *
 * Created on: 4 Nov 2017
 * Author: twig
 */

#include "LEDs.h"

void InitLeds(){
    HAL_TIM_Base_Init(hardware.htim1);
    HAL_TIM_Base_Start(hardware.htim1);
    HAL_TIM_PWM_Start(hardware.htim1,TIM_CHANNEL_2);
    HAL_TIM_Base_Init(hardware.htim3);
    HAL_TIM_Base_Start(hardware.htim3);
    HAL_TIM_PWM_Start(hardware.htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(hardware.htim3,TIM_CHANNEL_1);
	SetAllLedOff();
}


void SetRLed(int16_t darkness){
	__HAL_TIM_SET_COMPARE(hardware.htim1, TIM_CHANNEL_2,darkness);
}

void SetGLed(int16_t darkness){
	__HAL_TIM_SET_COMPARE(hardware.htim3, TIM_CHANNEL_1,darkness);
}

void SetBLed(int16_t darkness){
    __HAL_TIM_SET_COMPARE(hardware.htim3, TIM_CHANNEL_2,darkness);
}


void SetAllLedOff(){
	SetRLed(0xFFFF);
	SetGLed(0xFFFF);
	SetBLed(0xFFFF);
}
