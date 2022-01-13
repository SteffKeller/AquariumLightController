#include "LightImpl.hpp"
#include <string>

using namespace std;

LightImpl::LightImpl()
{
    _state = 1;
}

LightImpl::LightImpl(uint8_t fastLEDPos)
{
    _fastLEDPos = fastLEDPos;
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

void LightImpl::processTime(String actualTime)
{
    // erase(actualTime, ':');
    // int acutalTimeInt = std::stoi(actualTime);
}

uint8_t LightImpl::getFastLedPos()
{
    return _fastLEDPos;
}
