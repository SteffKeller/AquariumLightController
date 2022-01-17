#ifndef LIGHTIMPL_H
#define LIGHTIMPL_H
#pragma once

#include <Arduino.h>
#include <ezTime.h>
#include <TimeHelper.hpp>
class LightImpl
{
private:
	String _lightName;
	uint8_t _fastLEDPos;
	bool _state;
	bool _overrideOn;
	Timezone *_actualTime;

public:
	LightImpl(String lightName, uint8_t fastLEDPos, Timezone &actualTime);
	LightImpl();
	~LightImpl();

	void computeLight();
	void process();
	void setTimes(String onTime, String offTime, String dimmValueIn);
	void overrideState(bool overrideOn, bool overrideState);
	uint8_t getState();
	uint8_t getFastLedPos();
	String getLightName();

	tmElements_t mOnTime[2];
	tmElements_t mOffTime[2];
	uint8_t mDimmValueOn;
};
#endif