#include "LightJsonConverter.hpp"

LightJsonConverter::LightJsonConverter()
{
}

LightJsonConverter::~LightJsonConverter()
{
}

void LightJsonConverter::convertLightToJson(LightImpl &light)
{
    TimeHelper timeHelper = TimeHelper();
    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use https://arduinojson.org/assistant to compute the capacity.
    DynamicJsonDocument doc(1024);

    // Set the values in the document
    doc["name"] = light.getLightName();
    doc["lon"][0] = timeHelper.makeTimeString(light.mOnTime[0]);
    doc["lon"][1] = timeHelper.makeTimeString(light.mOnTime[1]);
    doc["loff"][0] = timeHelper.makeTimeString(light.mOffTime[0]);
    doc["loff"][1] = timeHelper.makeTimeString(light.mOffTime[1]);
    doc["dimtime"] = light.mDimmTime;
    doc["dim"] = light.mDimmValueOn;

    Serial.println("bla");
    // Lastly, you can print the resulting JSON to a String
    String output;
    serializeJson(doc, output);
    Serial.println(output);

    String path = "/" + light.getLightName() + ".json";
    File file = SPIFFS.open(path, "w");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    int bytesWritten = file.print(output);

    if (bytesWritten > 0)
    {
        Serial.println("File was written");
        Serial.println(bytesWritten);
    }
    else
    {
        Serial.println("File write failed");
    }
}
void LightJsonConverter::getLight(LightImpl &light)
{
    String path = "/" + light.getLightName() + ".json";
    File file = SPIFFS.open(path, "w");
    file = SPIFFS.open(path);
    Serial.println("File Content:");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}
