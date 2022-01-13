#include "LightToFastLEDConverter.hpp"
LighttoFastLEDConverter::LighttoFastLEDConverter()
{
}

LighttoFastLEDConverter::LighttoFastLEDConverter(CFastLED &fastLed, LightImpl &light1, LightImpl &light2, LightImpl &light3)

{
    fastLedInstance = &fastLed;
    _lights[0] = &light1;
    _lights[1] = &light2;
    _lights[2] = &light3;
}
void LighttoFastLEDConverter::controlLights(ControllerState actualstate)
{
    uint32_t fastLedColor = 0;
    switch (actualstate)
    {
    case ControllerState::off:
        fastLedInstance->showColor(0x00, 0); /// all lights off
        break;
    case ControllerState::on:
        fastLedInstance->showColor(0xFFFFFF, 255); /// all lights on
        break;
    case ControllerState::automatic:
        for (int i = 0; i < NUM_LIGHTS; i++)
        {
            uint8_t pos = _lights[i]->getFastLedPos();
            fastLedColor |= _lights[i]->mDimmValueOn << (8 * pos);
        }
        fastLedInstance->showColor(fastLedColor, MAX_BRIGHTNESS); /// all lights according actual light state
        break;
    default:
        break;
    }
}
LighttoFastLEDConverter::~LighttoFastLEDConverter()
{
    fastLedInstance->showColor(0x00, 0);
}