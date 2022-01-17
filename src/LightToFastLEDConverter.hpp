#ifndef LIGHTTOFASTLEDCONVERTER_H
#define LIGHTTOFASTLEDCONVERTER_H
#pragma once

#include "FastLED.h"
#include "LightImpl.hpp"
#include "ControllerState.hpp"

#define NUM_LIGHTS 3
#define MAX_BRIGHTNESS 255
class LighttoFastLEDConverter
{
private:
	CLEDController *fastLedInstance;
	LightImpl *_lights[NUM_LIGHTS];

public:
	LighttoFastLEDConverter(CLEDController &fastLed, LightImpl (&lightsImpl)[NUM_LIGHTS]);
	~LighttoFastLEDConverter();
	void controlLights(ControllerState actualstate);
};
#endif