#ifndef SIDELED_H_   /* Include guard */
#define SIDELED_H_

#define SIDELED_NUM_LEDS 30
#define SIDELED_DATA_PIN 14

#define SIDELED_STATE_OFF 0
#define SIDELED_STATE_ON 1
#define SIDELED_STATE_BLINK 2
#define SIDELED_STATE_BLINK_INVERS 3
#define SIDELED_STATE_FADE 3
#define SIDELED_STATE_FADE_RG 5
#define SIDELED_STATE_FADE_GB 6
#define SIDELED_STATE_FADE_BR 7


#include <FastLED.h>

void init_sideled();

//void set_sideled_state(uint8_t led_start, uint8_t led_middle, uint8_t led_end, uint8_t state);

//void set_sideled_color(uint8_t led_start, uint8_t led_middle, uint8_t led_end, CRGB color);

void set_sideled_state(uint8_t led_start, uint8_t led_end, uint8_t state);

void set_sideled_color(uint8_t led_start, uint8_t led_end, CRGB color);


void blink_sideleds(uint8_t led_livingroom, uint8_t led_bedroom, uint8_t led_attic, CRGB color, uint16_t onDuration, uint16_t offDuration);



#endif /* SIDELED_H_ */