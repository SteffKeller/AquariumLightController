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
        _state |= calculator.calcTimeBetween(&mOnTime[1], &mOffTime[1], &actTime);
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
void LightImpl::loadValuesFromJsonString(String jsonString)
{
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonString);
    TimeHelper timeHelper;

    // Test if parsing succeeds.
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    if (doc.containsKey("name"))
    {
        _lightName = doc["name"].as<String>();
    }

    if (doc.containsKey("dimtime"))
    {
        mDimmTime = doc["dimtime"];
    }

    if (doc.containsKey("dim"))
    {
        mDimmValueOn = doc["dim"];
    }
    for (size_t i = 0; i < 2; i++)
    {
        if (doc.containsKey("lon"))
        {
            mOnTime[i] = timeHelper.makeTmElement(doc["lon"][i]);
        }
        if (doc.containsKey("loff"))
        {
            mOffTime[i] = timeHelper.makeTmElement(doc["loff"][i]);
        }
    }
}
