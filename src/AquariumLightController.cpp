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

//NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, +1);
String l1onTime, acutalTime;

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
const char *PARAM_L1OFF = "l1off";
const char *PARAM_L2ON = "l2on";
const char *PARAM_L2OFF = "l2off";
const char *PARAM_MLON = "mlon";
const char *PARAM_MLOFF = "mloff";

//Display
uint8_t DisBuff[2 + 5 * 5 * 3]; //Used to store RBG color values

//Declaration
String processor(const String &var);
void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata); //Set the colors of LED, and save the relevant data to DisBuff[].

void setup()
{
  // Serial port for debugging purposes
  // Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  //Init Atom-Matrix(Initialize serial port, LED).
  M5.begin(true, false, true);
  delay(10); //delay10ms.  延迟10ms
  setBuff(0xff, 0x00, 0x00);
  M5.dis.displaybuff(DisBuff);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  timeClient.setTimeOffset(2);

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

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
  uint8_t FSM = 0; //Store the number of key presses.

  // delay(1000);
  // get ntp time
  timeClient.setTimeOffset(5);
  timeClient.update();
  acutalTime = timeClient.getFormattedTime();
  // Serial.println(timeClient.getFormattedTime());

  if (M5.Btn.wasPressed())
  { //Check if the key is pressed.
    switch (FSM)
    {
    case 0:
      setBuff(0x40, 0x00, 0x00);
      break;
    case 1:
      setBuff(0x00, 0x40, 0x00);
      break;
    case 2:
      setBuff(0x00, 0x00, 0x40);
      break;
    case 3:
      setBuff(0x20, 0x20, 0x20);
      break;
    default:
      break;

      M5.dis.displaybuff(DisBuff);
      Serial.println("state" + FSM);
      FSM++;
      if (FSM >= 4)
      {
        FSM = 0;
      }
    }
    // delay(50);
    M5.update(); //Read the press state of the key.
  }
}
/**
 * @brief Process input messages values for display
 * 
 * @param var Message
 * @return String preceed value for message
 */
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
  if (var == "L1ON")
  {
    // l1onTime = timeClient.getFormattedTime();
    Serial.println("get l1on:" + l1onTime);
    return l1onTime;
  }
  if (var == "ACTUALTIME")
  {
    Serial.println("actual time:" + acutalTime);
    return acutalTime;
  }

  return String();
}

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{ //Set the colors of LED, and save the relevant data to DisBuff[].
  DisBuff[0] = 0x05;
  DisBuff[1] = 0x05;
  for (int i = 0; i < 25; i++)
  {
    DisBuff[2 + i * 3 + 0] = Rdata;
    DisBuff[2 + i * 3 + 1] = Gdata;
    DisBuff[2 + i * 3 + 2] = Bdata;
  }
}