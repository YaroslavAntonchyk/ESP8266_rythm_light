#include <FastLED.h>
#include <FixedPoints.h>
#include "user_interface.h"

#define LED_PIN     2
#define NUM_LEDS    58
#define SAMPLES     50
#define MIN_LED_LEVEL 20
#define MAX_LED_LEVEL 255

CRGB leds[NUM_LEDS];
uint16_t volume_level = 0;
uint16_t volume_level_max = 0;
uint16_t abs_volume_level = 0;
uint8_t out_volume_level = 0;
float volume_level_filtred = 500.0F;
float mean_volume = 420;
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
    mean_volume = mean_volume*0.999 + static_cast<float>(volume_level)*0.001;
    abs_volume_level = abs(volume_level - static_cast<uint16_t>(mean_volume));

    if(volume_level_max < abs_volume_level)
      volume_level_max = abs_volume_level;
  }
  // low pass filter
  volume_level_filtred = static_cast<float>(volume_level_max)*0.07 + volume_level_filtred*0.93;
  out_volume_level = map(static_cast<uint16_t>(volume_level_filtred), 0, static_cast<uint16_t>(mean_volume), MIN_LED_LEVEL, MAX_LED_LEVEL);
  out_volume_level = static_cast<uint8_t>(constrain(out_volume_level, MIN_LED_LEVEL, MAX_LED_LEVEL));
  plot_level(static_cast<uint16_t>(out_volume_level));

  for(int k = 0; k < NUM_LEDS; k++)
    leds[k] = CRGB(0, out_volume_level, out_volume_level); 
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