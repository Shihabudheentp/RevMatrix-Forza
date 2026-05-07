#include <WiFi.h>
#include <WiFiUdp.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

//replace with ur wifi

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

//UDP

WiFiUDP udp;
const int UDP_PORT = 8000;

byte packetBuffer[512];



#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN 12
#define CS_PIN   13
#define CLK_PIN  14

MD_Parola display = MD_Parola(
  HARDWARE_TYPE,
  DATA_PIN,
  CLK_PIN,
  CS_PIN,
  MAX_DEVICES
);


MD_MAX72XX* mx = display.getGraphicObject();



#define LED_PIN     4
#define NUM_LEDS    12
#define BRIGHTNESS  90

CRGB leds[NUM_LEDS];

//variables

float speed;
float currentRPM;
float maxRPM;

int gear;

char speedText[10];
char gearText[5];

unsigned long lastBlink = 0;
bool blinkState = true;



void showBootMessage(const char* text)
{
  display.displayClear();

  display.displayText(
    text,
    PA_CENTER,
    60,
    1000,
    PA_SCROLL_LEFT,
    PA_SCROLL_LEFT
  );

  while (!display.displayAnimate())
  {
  }
}



void setup()
{
  Serial.begin(115200);

  display.begin(2);
  display.setZone(0, 0, 1);
  display.setZone(1, 3, 3);


  for (int i = 0; i < 2; i++)
  {
    display.setZoneEffect(i, true, PA_FLIP_UD);
    display.setZoneEffect(i, true, PA_FLIP_LR);
  }

  display.setIntensity(1);
  display.displayClear();



  for (int i = 0; i < MAX_DEVICES; i++)
  {
    mx->transform(i, MD_MAX72XX::TFUD);
    mx->transform(i, MD_MAX72XX::TFLR);
  }



  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness(BRIGHTNESS);

  FastLED.clear();
  FastLED.show();



  showBootMessage("FORZA");



  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  showBootMessage("CONNECTED");

  udp.begin(UDP_PORT);

  Serial.println("Listening for telemetry...");
}



void drawRPMBar(float rpmPercent)
{
  // Clear RPM module
  for (int col = 16; col < 24; col++)
  {
    mx->setColumn(col, 0x00);
  }

  int bars = map(
    rpmPercent * 100,
    0,
    82,
    0,
    5
  );

  if (bars < 0)
    bars = 0;

  if (bars > 5)
    bars = 5;


  byte barPattern = B01111110;


  for (int i = 0; i < bars; i++)
  {
    mx->setColumn(18 + i, barPattern);
  }
}



void drawLEDRPM(float rpmPercent, bool redline)
{
  FastLED.clear();



  if (redline)
  {
    if (blinkState)
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Red;
      }
    }

    FastLED.show();
    return;
  }



  int activeLEDs = map(
    rpmPercent * 100,
    0,
    82,
    0,
    NUM_LEDS
  );



  if (activeLEDs < 0)
    activeLEDs = 0;

  if (activeLEDs > NUM_LEDS)
    activeLEDs = NUM_LEDS;



  for (int i = 0; i < activeLEDs; i++)
  {


    if (i < 4)
    {
      leds[i] = CRGB::Green;
    }



    else if (i < 8)
    {
      leds[i] = CRGB::Yellow;
    }


    else
    {
      leds[i] = CRGB::Red;
    }
  }

  FastLED.show();
}

void loop()
{
  int packetSize = udp.parsePacket();

  if (packetSize)
  {
    udp.read(packetBuffer, 512);

    memcpy(&speed, packetBuffer + 256, 4);
    speed *= 3.6;

    memcpy(&maxRPM, packetBuffer + 8, 4);
    memcpy(&currentRPM, packetBuffer + 16, 4);

    float rpmPercent = currentRPM / maxRPM;

    gear = packetBuffer[319];

    sprintf(speedText, "%3d", (int)speed);

    if (gear == 255)
      sprintf(gearText, "R");
    else if (gear == 0)
      sprintf(gearText, "N");
    else
      sprintf(gearText, "%d", gear);

    bool redline = rpmPercent > 0.82;

    if (redline)
    {
      if (millis() - lastBlink > 55)
      {
        blinkState = !blinkState;
        lastBlink = millis();
      }
    }
    else
    {
      blinkState = true;
    }

    if (blinkState)
    {
      display.displaySuspend(false);

      // SPEED

      display.displayZoneText(
        0,
        speedText,
        PA_RIGHT,
        0,
        0,
        PA_PRINT,
        PA_NO_EFFECT
      );

      // GEAR

      display.displayZoneText(
        1,
        gearText,
        PA_CENTER,
        0,
        0,
        PA_PRINT,
        PA_NO_EFFECT
      );

      display.displayAnimate();

      // RPM BAR

      drawRPMBar(rpmPercent);
    }
    else
    {
      // FULL DISPLAY OFF

      display.displaySuspend(true);

      mx->clear();
    }

    // -------- WS2812B --------

    drawLEDRPM(rpmPercent, redline);

    // -------- SERIAL --------

    Serial.print("Speed: ");
    Serial.print(speedText);

    Serial.print(" | Gear: ");
    Serial.print(gearText);

    Serial.print(" | RPM%: ");
    Serial.println(rpmPercent);
  }
}