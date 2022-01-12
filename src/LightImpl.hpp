#ifndef LIGHTIMPL_H
#define LIGHTIMPL_H
#pragma once

#include <Arduino.h>

class LightImpl
{
private:
	uint8_t _fastLEDPos;
	bool _state;

public:
	LightImpl(uint8_t fastLEDPos);
	LightImpl();
	~LightImpl();

	void computeLight();
	void processTime(String actualTime);
	void setTimes(String onTime, String offTime, String dimmValueIn);
	void overrideState(bool overrideOn, bool overrideState);
	uint8_t getState();

	String mOnTime;
	String mOffTime;
	uint8_t mDimmValueOn;
};
#endif