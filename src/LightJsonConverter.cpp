#include "LightJsonConverter.hpp"
#include <Arduino.h>

LightJsonConverter::LightJsonConverter()
{
}

LightJsonConverter::~LightJsonConverter()
{
}

String LightJsonConverter::convertLightToJson(LightImpl &light)
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

    String output;
    serializeJson(doc, output);
    return output;
}

void LightJsonConverter::storeJsonString(String jsonLight, String filename)
{
    File file = SPIFFS.open(filename, "w");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    int bytesWritten = file.print(jsonLight);
    file.close();
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

String LightJsonConverter::loadJsonString(String filename)
{
    String serialJson;
    File file = SPIFFS.open(filename, "r");
    if (!file)
    {
        Serial.println("Error opening file");
    }
    Serial.println("File load json:");
    while (file.available())
    {

        // Serial.write(file.read());
        serialJson = file.readString();
    }
    file.close();

    Serial.println("file loaded" + serialJson);

    

    return serialJson;
}


void LightJsonConverter::getLight(LightImpl &light)
{
    // String path = "/" + light.getLightName() + ".json";
    // File file = SPIFFS.open(path, "w");
    // file = SPIFFS.open(path);
    // Serial.println("File Content:");
    // while (file.available())
    // {
    //     Serial.write(file.read());
    // }
    // file.close();
}
