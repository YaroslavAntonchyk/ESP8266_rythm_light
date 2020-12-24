#include <FastLED.h>
#include <FixedPoints.h>
#include "user_interface.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define LED_PIN     2
#define NUM_LEDS    58
#define SAMPLES     50

const char* ssid = "MARVEL";
const char* password = "88280718";

uint8_t MIN_LED_LEVEL = 20;
uint8_t MAX_LED_LEVEL = 255;

CRGB leds[NUM_LEDS];
WiFiServer server(80);

uint16_t volume_level = 0;
uint16_t volume_level_max = 0;
uint16_t abs_volume_level = 0;
uint8_t out_volume_level = 0;
float out_level_norm = 0.0F;
float volume_level_filtred = 500.0F;
float mean_volume = 420.0F;

float color[][3] = {{0.0F, 128.0F, 128.0F},
                    {128.0F, 0.0F, 128.0F}
                   };
uint8_t current_color = 1;
uint8_t r, g, b;

// for debuging 
uint64_t t;
int cnt = 0;


void setup() 
{
  Serial.begin(250000);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // if (MDNS.begin("ant_lab")) {
  //   Serial.println("MDNS responder started");
  // }

  server.begin();
  Serial.println("HTTP server started");

//   t = micros();
}

void loop() 
{
  WiFiClient client = server.available();

  if(client) 
  {
    Serial.println("New Client.");
    client.setTimeout(1000); 
    client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nHello world! "));
    client.stop();
    Serial.println("Client disconnected.");
  }
  else
  {
    rythm_light();
  }
  // MDNS.update();

}

void rythm_light()
{
  // max filter 
  volume_level_max = 0;
  for(uint8_t i = 0; i < SAMPLES; i++)
  {
    // volume_level = system_adc_read();
    volume_level = 9;
    // calculate mean value 
    mean_volume = mean_volume*0.999 + static_cast<float>(volume_level)*0.001;
    abs_volume_level = abs(volume_level - static_cast<uint16_t>(mean_volume));

    if(volume_level_max < abs_volume_level)
      volume_level_max = abs_volume_level;
  }
  // low pass filter
  volume_level_filtred = static_cast<float>(volume_level_max)*0.05 + volume_level_filtred*0.95;
  out_volume_level = map(static_cast<uint16_t>(volume_level_filtred), 0, static_cast<uint16_t>(mean_volume), MIN_LED_LEVEL, MAX_LED_LEVEL);
  out_volume_level = constrain(out_volume_level, MIN_LED_LEVEL, MAX_LED_LEVEL);
  // normilize volume level
  out_level_norm = static_cast<float>(out_volume_level) / static_cast<float>(MAX_LED_LEVEL);
  // plot_level(static_cast<uint16_t>(out_volume_level));
  r = static_cast<uint8_t>(color[current_color][0]*out_level_norm);
  g = static_cast<uint8_t>(color[current_color][1]*out_level_norm);
  b = static_cast<uint8_t>(color[current_color][2]*out_level_norm);
  for(int k = 0; k < NUM_LEDS; k++)
    leds[k] = CRGB(r, g, b); 
  FastLED.show();

  // Serial.println(micros() - t); 
  // t = micros();
}

void plot_level(uint16_t level)
{
  Serial.print(0);  // To freeze the lower limit
  Serial.print(" ");
  Serial.print(1000);  // To freeze the upper limit
  Serial.print(" ");
  Serial.println(level);
}