#ifndef LIGHTJSONCONVERTER_H
#define LIGHTJSONCONVERTER_H
#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include <LightImpl.hpp>
#include <TimeHelper.hpp>
#include "SPIFFS.h"

class LightJsonConverter
{
private:
public:
	LightJsonConverter();
	~LightJsonConverter();
	void convertLightToJson(LightImpl &light);
	void getLight(LightImpl &light);
};
#endif