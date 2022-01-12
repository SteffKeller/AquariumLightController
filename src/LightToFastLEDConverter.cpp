#include "LightToFastLEDConverter.hpp"
LighttoFastLEDConverter::LighttoFastLEDConverter()
{
}

LighttoFastLEDConverter::LighttoFastLEDConverter(CFastLED &fastledInstance)
{
    *fastLedInstance = fastledInstance;
}

LighttoFastLEDConverter::~LighttoFastLEDConverter()
{
}