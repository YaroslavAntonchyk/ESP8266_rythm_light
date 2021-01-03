#include <FastLED.h>
#include <FixedPoints.h>
#include "user_interface.h"

#define LED_PIN     2
#define NUM_LEDS    58
#define SAMPLES     50

uint8_t MIN_LED_LEVEL = 20;
uint8_t MAX_LED_LEVEL = 255;

CRGB leds[NUM_LEDS];

float volume_level_filtred = 500.0F;
float mean_volume = 420.0F;
float bg_level = 0.0784F; // MIN_LED_LEVEL / 255 back graund level 

float color[][3] = {{0.0F, 128.0F, 128.0F},
                    {128.0F, 0.0F, 128.0F},
                    {13.0F, 184.0F, 169.0F},
                    {72.0F, 255.0F, 0.0F},
                    {0.0F, 229.0F, 255.0F},
                    {0.0F, 166.0F, 255.0F},
                    {191.0F, 0.0F, 255.0F},
                    {255.0F, 0.0F, 93.0F},
                    {200.0F, 255.0F, 0.0F},
                    {0.0F, 255.0F, 204.0F},
                    {255.0F, 255.0F, 255.0F}
                   };

uint8_t current_color = 4;
uint8_t mode = 1;

// for debuging 
uint64_t t;
// int cnt = 0;

void setup() 
{
  Serial.begin(250000);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  t = micros();
  // Serial.println(micros() - t); 
  // t = micros();
}

void loop() 
{
  if((micros() - t) > 300000000) //every 5 mins 
  {
    t = micros();
    current_color = (current_color < 9) ? (current_color + 1) : 0;
  }
  if(t > micros())
    t = micros(); 

  switch(mode)
  {
    case 1 :
      rythm_light_blink(current_color, vu_meter());
      break;
    case 2 :
      rythm_light_expand(current_color, vu_meter());
      break;
    default:
      simple_flash(0, 0.5F);
  }
}

// return filtred and normilized volume level 
float vu_meter() // return 
{
  uint16_t volume_level = 0;
  uint16_t abs_volume_level = 0;
  uint16_t out_volume_level = 0;
  uint16_t volume_level_max = 0;

  // max filter 
  for(uint8_t i = 0; i < SAMPLES; i++)
  {
    //volume_level = system_adc_read();
    volume_level = analogRead(A0);
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
  float out_level_norm = static_cast<float>(out_volume_level) / static_cast<float>(MAX_LED_LEVEL);

  // plot_level(static_cast<uint16_t>(out_volume_level));
  return out_level_norm;
  
}

void rythm_light_blink(uint8_t color_idx, float volume_level)
{
  uint8_t r = static_cast<uint8_t>(color[color_idx][0]*volume_level);
  uint8_t g = static_cast<uint8_t>(color[color_idx][1]*volume_level);
  uint8_t b = static_cast<uint8_t>(color[color_idx][2]*volume_level);
  for(int k = 0; k < NUM_LEDS; k++)
    leds[k] = CRGB(r, g, b); 
  FastLED.show();
}

void rythm_light_expand(uint8_t color_idx, float volume_level)
{
  uint8_t led_length = static_cast<uint8_t>(volume_level*(NUM_LEDS/2));
  uint8_t r = static_cast<uint8_t>(color[color_idx][0]);
  uint8_t g = static_cast<uint8_t>(color[color_idx][1]);
  uint8_t b = static_cast<uint8_t>(color[color_idx][2]);

  uint8_t r_bg = static_cast<uint8_t>(color[color_idx][0]*bg_level);
  uint8_t g_bg = static_cast<uint8_t>(color[color_idx][1]*bg_level);
  uint8_t b_bg = static_cast<uint8_t>(color[color_idx][2]*bg_level);

  for(uint8_t i = 0; i < NUM_LEDS; i++)
  {
    if((i < NUM_LEDS/2 - led_length) || (i >= NUM_LEDS/2 + led_length))
      leds[i] = CRGB(r_bg, g_bg, b_bg);
    else 
      leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

void simple_flash(uint8_t color_idx, float brightness)
{
  uint8_t r = static_cast<uint8_t>(color[color_idx][0]*brightness);
  uint8_t g = static_cast<uint8_t>(color[color_idx][1]*brightness);
  uint8_t b = static_cast<uint8_t>(color[color_idx][2]*brightness);
  for(int k = 0; k < NUM_LEDS; k++)
    leds[k] = CRGB(r, g, b); 
  FastLED.show();
  delay(100);
}

void plot_level(uint16_t level)
{
  Serial.print(0);  // To freeze the lower limit
  Serial.print(" ");
  Serial.print(1000);  // To freeze the upper limit
  Serial.print(" ");
  Serial.println(level);
}