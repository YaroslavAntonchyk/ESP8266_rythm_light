#include <FastLED.h>
#include <FixedPoints.h>
#include "user_interface.h"

#define LED_PIN     2
#define NUM_LEDS    58
#define SAMPLES     50

uint8_t MIN_LED_LEVEL = 20;
uint8_t MAX_LED_LEVEL = 255;

CRGB leds[NUM_LEDS];
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


void setup() 
{
  Serial.begin(250000);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
//   t = micros();
}

void loop() 
{
  // Serial.println(micros() - t); 
  
  // max filter 
  volume_level_max = 0;
  for(uint8_t i = 0; i < SAMPLES; i++)
  {
    volume_level = system_adc_read();
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