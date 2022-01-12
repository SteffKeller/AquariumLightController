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

void LightImpl::processTime(String actualTime)
{
    // erase(actualTime, ':');
    // int acutalTimeInt = std::stoi(actualTime);
}