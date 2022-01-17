#include "LightImpl.hpp"
#include <string>

using namespace std;

LightImpl::LightImpl()
{
    _state = 1;
}

LightImpl::LightImpl(String lightName, uint8_t fastLEDPos, Timezone &actualTime)
{
    _fastLEDPos = fastLEDPos;
    _actualTime = &actualTime;
    _lightName = lightName;
}

LightImpl::~LightImpl()
{
}

uint8_t LightImpl::getState()
{
    return _state;
}

void LightImpl::overrideState(bool overrideOn, bool overrideState)
{
    _overrideOn = overrideOn;
    if (_overrideOn)
    {
        _state = overrideState;
    }
}

void LightImpl::process()
{
    TimeHelper calculator;
    tmElements_t actTime;

    if (!_overrideOn)
    {

        breakTime(_actualTime->tzTime(), actTime);
        _state = calculator.calcTimeBetween(&mOnTime[0], &mOffTime[0], &actTime);
        //_state |= calculator.calcTimeBetween(&mOnTime[1], &mOffTime[1], &actTime);
    }
}

uint8_t LightImpl::getFastLedPos()
{
    return _fastLEDPos;
}

String LightImpl::getLightName()
{
    return _lightName;
}
