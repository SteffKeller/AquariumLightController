#ifndef LIGHTTOFASTLEDCONVERTER_H
#define LIGHTTOFASTLEDCONVERTER_H
#pragma once

#include "FastLED.h"

class LighttoFastLEDConverter
{
private:
	CFastLED *fastLedInstance;

public:
	LighttoFastLEDConverter();
	LighttoFastLEDConverter(CFastLED &fastLed);
	~LighttoFastLEDConverter();
};
#endif