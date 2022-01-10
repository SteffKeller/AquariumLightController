/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Compile SPIFFS & upload
//Plaform.io cli ->
// pio run --target uploadfs
// Import required libraries

#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

//NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Replace with your network credentials
const char *ssid = "diveintothenet";
const char *password = "dtn24steffshome67L";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Form Inputs
const char *PARAM_INPUT_3 = "input3";
const char *PARAM_L1ON = "l1on";

//Declaration
String processor(const String &var);

// Replaces placeholder with LED state value
String processor(const String &var)
{
  Serial.println(var);
  if (var == "STATE")
  {
    if (digitalRead(ledPin))
    {
      ledState = "ON";
    }
    else
    {
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  // if (var == "l1on")
  // {
  //   Serial.print(var);
  //   return var;
  // }

  return String();
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // get ntp time
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin, HIGH);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin, LOW);
              request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // Route to set l1on
  // server.on("/l1on", HTTP_GET, [](AsyncWebServerRequest *request)
  //           {
  //             Serial.println("l1on print");
  //             timeClient.update();
  //             Serial.println(timeClient.getFormattedTime());
  //             request->send(SPIFFS, "/index.html", String(), false, processor);
  //           });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String inputMessage;
              String inputParam;
              // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
              if (request->hasParam(PARAM_INPUT_3))
              {
                inputMessage = request->getParam(PARAM_INPUT_3)->value();
                inputParam = PARAM_INPUT_3;
              }
              // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
              else if (request->hasParam(PARAM_L1ON))
              {
                inputMessage = request->getParam(PARAM_L1ON)->value();
                inputParam = PARAM_L1ON;
              }
              else
              {
                inputMessage = "No message sent";
                inputParam = "none";
              }
              Serial.println(inputMessage);
              request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam + ") with value: " + inputMessage + "<br><a href=\"/\">Return to Home Page</a>");
            });
  // Start server
  server.begin();
}

void loop()
{
}