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
	String convertLightToJson(LightImpl &light);
	void getLight(LightImpl &light);
	void storeJsonString(String jsonLight, String filename);
	// LightJsonConverter loadJson(String filename);
	String loadJsonString(String filename);
};
#endif