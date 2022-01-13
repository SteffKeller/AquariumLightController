/*********
  Stefan Keller 2022/01
  based on the tutorial of Rui Santos https://randomnerdtutorials.com  
*********/

// Compile SPIFFS & upload
// Plaform.io cli ->
// pio run --target uploadfs

// Import required libraries
#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "M5Atom.h"
#include "LightImpl.hpp"
#include "time.h"
#include "FastLED.h"
#include "ControllerState.hpp"
#include "LightToFastLEDConverter.hpp"
#include <time.h>
#include <arduino-timer.h>

//NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String l1onTime, l1offTime, l2onTime, l2offTime, actualTime;

// Timer
auto timer10s = timer_create_default();            // create a timer with default settings
auto timer1s = timer_create_default();             // create a timer with default settings
auto timerDisplayTimeout = timer_create_default(); // create a timer with default settings

// network credentials
const char *ssid = "diveintothenet";
const char *password = "dtn24steffshome67L";

// Set LED GPIO
#define P9813_C_PIN 21
#define P9813_D_PIN 25
#define P9813_NUM_LEDS 1
// This is an array of leds.  One item for each led in your strip.
CRGB fastLedDummyLed[P9813_NUM_LEDS];
LighttoFastLEDConverter *lightConverter;

const int ledPin = 2;
// Stores LED state
String ledState;

// light controller instances
LightImpl light1, light2, lightMl;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Form Inputs
const char *PARAM_INPUT_3 = "input3";
const char *PARAM_L1ON = "l1on";
const char *PARAM_L1OFF = "l1off";
const char *PARAM_L2ON = "l2on";
const char *PARAM_L2OFF = "l2off";
const char *PARAM_MLON = "mlon";
const char *PARAM_MLOFF = "mloff";

ControllerState fsmState = on; //Store the number of key presses.

//Display
uint8_t DisBuff[4 + 25 * 3]; //Used to store RBG color values

//Declaration
String webServerProcessor(const String &var);
void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata); //Set the colors of LED, and save the relevant data to DisBuff[].
String stateToString(ControllerState state);               //Convert the state to an string to display
bool timeUpdate(void *);
bool displayTimeoutCallback(void *);

void setup()
{
  pinMode(ledPin, OUTPUT);
  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // create the lights
  light1 = LightImpl(0);
  light2 = LightImpl(1);
  lightMl = LightImpl(2);
  lightConverter = new LighttoFastLEDConverter{FastLED, light1, light2, lightMl};

  light1.mOnTime = "11:30";
  light1.mOffTime = "22:45";

  //Init Atom-Matrix(Initialize serial port, LED).
  M5.begin(true, false, true);
  M5.dis.begin(25);
  delay(10);
  setBuff(0xff, 0x00, 0x00);
  M5.dis.displaybuff(DisBuff);

  // Init LED outputs
  FastLED.addLeds<P9813, P9813_D_PIN, P9813_C_PIN, RGB>(fastLedDummyLed, P9813_NUM_LEDS); // BGR ordering is typical

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // call the toggle_led function every 1000 millis (1 second)
  timer10s.every(1000, timeUpdate);
  timerDisplayTimeout.in(10000, displayTimeoutCallback);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, webServerProcessor); });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // Send a GET request to <ESP_IP>/get?>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String inputMessage;
              String inputParam;
              if (request->hasParam(PARAM_INPUT_3))
              {
                inputMessage = request->getParam(PARAM_INPUT_3)->value();
                inputParam = PARAM_INPUT_3;
              }
              if (request->hasParam(PARAM_L1ON))
              {
                light1.mOnTime = request->getParam(PARAM_L1ON)->value();
              }
              if (request->hasParam(PARAM_L1OFF))
              {
                light1.mOffTime = request->getParam(PARAM_L1OFF)->value();
                inputParam = PARAM_L1OFF;
              }
              else
              {
                inputMessage = "No message sent";
                inputParam = "none";
              }
              timerDisplayTimeout.in(10000, timeUpdate);
              Serial.println(inputMessage);
              request->send(100, "/index.html");
            });
  // Start server
  server.begin();
}
uint8_t bright = 00;

void loop()
{
  // tick the timers
  timer10s.tick();
  timerDisplayTimeout.tick();

  lightConverter->controlLights(fsmState);

  if (M5.Btn.wasPressed())
  {
    //handle state change
    uint8_t state = static_cast<uint8_t>(fsmState);
    ++state > 2 ? state = 0 : state;
    fsmState = static_cast<ControllerState>(state);

    switch (fsmState)
    {
    case ControllerState::off:
      setBuff(0xAA, 0x00, 0x00);
      break;
    case ControllerState::on:
      setBuff(0x00, 0x40, 0x00);
      break;
    case ControllerState::automatic:
      setBuff(0x00, 0x00, 0x40);
      break;
    default:
      break;
    }
    M5.dis.displaybuff(DisBuff);
    Serial.printf("state %i", static_cast<uint8_t>(fsmState));

    // reset display timeout and set it new
    timerDisplayTimeout.cancel();
    timerDisplayTimeout.in(10000, displayTimeoutCallback);
  }
  M5.update(); //Read the press state of the key.
}

/**
 * @brief Process input messages values for display
 * 
 * @param var Message
 * @return String preceed value for message
 */
String webServerProcessor(const String &var)
{
  Serial.println(var);
  if (var == "L1ON")
  {
    return light1.mOnTime;
  }
  if (var == "L1OFF")
  {
    return light1.mOffTime;
  }
  if (var == "ACTUALTIME")
  {
    return actualTime;
  }
  if (var == "LIGHT1STATE")
  {
    return stateToString(static_cast<ControllerState>(light1.getState()));
  }
  if (var == "CONTROLLERSTATE")
  {
    return stateToString(fsmState);
  }

  return String();
}

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{ //Set the colors of LED, and save the relevant data to DisBuff[].
  // DisBuff[0] = 0x00;
  // DisBuff[1] = 0x00;
  for (int i = 0; i < 25; i++)
  {
    DisBuff[i * 3 + 0] = Rdata;
    DisBuff[i * 3 + 1] = Gdata;
    DisBuff[i * 3 + 2] = Bdata;
  }
}

/**
 * @brief compute the state of the controller to correct string for the webserver
 * 
 * @param state state to get the string from
 * @return String 
 */
String stateToString(ControllerState state)
{
  switch (state)
  {
  case ControllerState::on:
    return "ON";
    break;
  case ControllerState::off:
    return "OFF";
    break;
  case ControllerState::automatic:
    return "AUTO";
    break;
  default:
    return "unknown";
    break;
  }
}

bool timeUpdate(void *)
{
  timeClient.setTimeOffset(3600);
  timeClient.update();
  actualTime = timeClient.getFormattedTime();
  Serial.println(actualTime);
  return true;
}

bool displayTimeoutCallback(void *)
{
  Serial.println("off dis");
  setBuff(0x00, 0x00, 0x00);
  M5.dis.displaybuff(DisBuff);
  M5.dis.clear();
  M5.dis.setBrightness(0);
  return false;
}
