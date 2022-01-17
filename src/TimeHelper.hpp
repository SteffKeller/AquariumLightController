#ifndef TIMECALCULATOR_H
#define TIMECALCULATOR_H
#pragma once

#include <Arduino.h>
#include <ezTime.h>

class TimeHelper
{
private:
public:
	TimeHelper();
	~TimeHelper();
	bool calcTimeBetween(tmElements_t *onTime, tmElements_t *offTime, tmElements_t *actualTime);
	String makeTimeString(tmElements_t &time);
	tmElements_t makeTmElement(String timeString);
};
#endif