/*
 * LEDs.h
 *
 *  Created on: 4 Nov 2017
 *      Author: twig
 */

#ifndef SRC_LEDS_H_
#define SRC_LEDS_H_

#include "KeyboardFirmware.h"

#include <cinttypes>

void InitLeds();

void SetRLed(int16_t darkness);
void SetGLed(int16_t darkness);
void SetBLed(int16_t darkness);

void SetAllLedOff();


#endif /* SRC_LEDS_H_ */
