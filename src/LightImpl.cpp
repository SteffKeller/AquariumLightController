#include "LightImpl.hpp"

LightImpl::LightImpl()
{
    _state = 1;
}

LightImpl::~LightImpl()
{
}

uint8_t LightImpl::getState()
{
    return _state;
}