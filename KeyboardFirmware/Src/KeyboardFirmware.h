/*
 * KeyboardFirmware.h
 *
 *  Created on: 4 Nov 2017
 *      Author: twig
 */

#ifndef SRC_KEYBOARDFIRMWARE_H_
#define SRC_KEYBOARDFIRMWARE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"


struct Hardware {
	TIM_HandleTypeDef *htim1;
	TIM_HandleTypeDef *htim3;
	UART_HandleTypeDef *huart1;
};

extern struct Hardware hardware;


void KFMain();


#ifdef __cplusplus
}
#endif
#endif /* SRC_KEYBOARDFIRMWARE_H_ */
