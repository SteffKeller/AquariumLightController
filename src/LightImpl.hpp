#ifndef LIGHTIMPL_H
#define LIGHTIMPL_H
#pragma once

#include <Arduino.h>

class LightImpl
{
private:
	String _gpioOutput;
	bool _state;

public:
	LightImpl(String gpioOutput);
	LightImpl();
	~LightImpl();

	void setLightState(bool on);
	void processTime(String actualTime);
	void setTimes(String onTime, String offTime, String dimmValueIn);
	void overrideState(bool overrideOn, bool overrideState);
	uint8_t getState();

	String mOnTime;
	String mOffTime;
	uint8_t mDimmValueOn;
};
#endif