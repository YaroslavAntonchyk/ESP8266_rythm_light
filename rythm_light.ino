// #include <FastLED.h>
// #include <FixedPoints.h>
// #define LED_PIN     2
// #define NUM_LEDS    58

// #define OFFSET 360
// #define MAX 801
// #define MIN 0
// #define NOISE 15
// #define MAX_COEF 1.8

// CRGB leds[NUM_LEDS];
// float adc_read = 0;
// float cur_level = 0;
// float max_level = 0;

// float current_max = MAX/2;
// float level = 0;
// float level_f = 0;
// float percent_level = 0.0;
// float offset = 360;

// float averK = 0.006;


// int r, g, b;

// void setup() 
// {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
// }

// void loop() 
// {
//   max_level = 0;
//   for(byte i = 0; i < 50; i++)
//   {
//     adc_read = (float)analogRead(A0);
//     offset = offset*0.99 + adc_read*0.01;
//     cur_level = abs(adc_read - offset);
//     if(max_level < cur_level)
//     {
//       max_level = cur_level;
//     }
//   }
//   current_max = current_max*0.99 + max_level*0.01;
//   level_f = level_f*0.92 + max_level*0.08;
//   level = map(level_f, NOISE, current_max*1.8, 0, 255);
  
// //  percent_level = level / 255.0;
// //  r = (int)0*percent_level;
// //  g = (int)128*percent_level;
// //  b = (int)160*percent_level;
//   for(int k = 0; k < NUM_LEDS; k++)
//     leds[k] = CRGB(0, level, level); 
//   FastLED.show();
  
//   plot_level(level);
  
// }

// void plot_level(int level)
// {
//   Serial.print(0);  // To freeze the lower limit
//   Serial.print(" ");
//   Serial.print(255);  // To freeze the upper limit
//   Serial.print(" ");
//   Serial.println(level);
// }

