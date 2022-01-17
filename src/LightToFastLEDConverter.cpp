#include "LightToFastLEDConverter.hpp"

LighttoFastLEDConverter::LighttoFastLEDConverter(CLEDController &fastLed, LightImpl (&lightsImpl)[NUM_LIGHTS])

{
    fastLedInstance = &fastLed;

    for (size_t i = 0; i < NUM_LIGHTS; i++)
    {
        _lights[i] = &lightsImpl[i];
    }
}

LighttoFastLEDConverter::~LighttoFastLEDConverter()
{
    fastLedInstance->showColor(0x00, 0);
}

void LighttoFastLEDConverter::controlLights(ControllerState actualstate)
{
    uint32_t fastLedColor = 0;
    switch (actualstate)
    {
    case ControllerState::off:
        for (int i = 0; i < NUM_LIGHTS; i++)
        {
            _lights[i]->overrideState(true, false);
        }
        fastLedInstance->showColor(0x00, 0); /// all lights off
        break;
    case ControllerState::on:
        for (int i = 0; i < NUM_LIGHTS; i++)
        {
            _lights[i]->overrideState(true, true);
        }
        fastLedInstance->showColor(0xFFFFFF, MAX_BRIGHTNESS); /// all lights on
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
