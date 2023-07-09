#include "sideled.h"

CRGB leds_current[SIDELED_NUM_LEDS];
CRGB leds_color[SIDELED_NUM_LEDS];
uint8_t leds_state[SIDELED_NUM_LEDS];
uint8_t changed = false;
uint8_t block = false;

unsigned long next_blink = 0;
unsigned long next_effekt = 0;
TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;

void LEDtask(void *arg){
  while (1){
    if(changed) {
      for(int a = 0; a < 15; a++) {
        leds_current[a] = leds_color[a];
        if(leds_state[a] == SIDELED_STATE_OFF) {
          leds_current[a] = CRGB::Black;
        }
        if(leds_state[a] == SIDELED_STATE_FADE) {
          leds_state[a] = SIDELED_STATE_FADE_RG;
          leds_current[a].blue = 0x00;
          leds_current[a].red = 0xFF;
          leds_current[a].green = 0x00;
        }
      }
      for(int a = 15; a < 26; a++) {
        leds_current[a] = leds_color[a];
        if(leds_state[a] == SIDELED_STATE_OFF) {
          leds_current[a] = CRGB::Black;
        }
          leds_state[a] = SIDELED_STATE_ON;
          leds_current[a] = CRGB::White;
      }
      for(int a = 26; a < 30; a++) {
        leds_current[a] = leds_color[a];
        if(leds_state[a] == SIDELED_STATE_OFF) {
          leds_current[a] = CRGB::Black;
        }
        if(leds_state[a] == SIDELED_STATE_FADE) {
          leds_state[a] = SIDELED_STATE_FADE_RG;
          leds_current[a].blue = 0x00;
          leds_current[a].red = 0xFF;
          leds_current[a].green = 0x00;
        }
      }
      FastLED.show();
      changed = false;
      block = false;
    }
    if(next_blink < millis() && !block) {
      for(int a = 0; a < SIDELED_NUM_LEDS; a++) {
        if(leds_state[a] == SIDELED_STATE_BLINK_INVERS) {
          leds_current[a] = CRGB::Black;
          leds_state[a] = SIDELED_STATE_BLINK;
        }
        else if(leds_state[a] == SIDELED_STATE_BLINK) {
          leds_current[a] = leds_color[a];
          leds_state[a] = SIDELED_STATE_BLINK_INVERS;
        }
      }
      FastLED.show();
      next_blink = millis() + 500;
    }
    if(next_effekt < millis() && !block) {
      for(int a = 0; a < SIDELED_NUM_LEDS; a++) {
        if(leds_state[a] == SIDELED_STATE_FADE_RG) {
          leds_current[a].red -= 10;
          leds_current[a].green += 10;
          if(leds_current[a].red <= 10) {
            leds_state[a] = SIDELED_STATE_FADE_GB;
          }
        }
        else if(leds_state[a] == SIDELED_STATE_FADE_GB) {
          leds_current[a].green -= 10;
          leds_current[a].blue += 10;
          if(leds_current[a].green <= 10) {
            leds_state[a] = SIDELED_STATE_FADE_BR;
          }
        }
        else if(leds_state[a] == SIDELED_STATE_FADE_BR) {
          leds_current[a].blue -= 10;
          leds_current[a].red += 10;
          if(leds_current[a].blue <= 10) {
            leds_state[a] = SIDELED_STATE_FADE_RG;
          }
        }
      }
      FastLED.show();
      next_effekt = millis() + 50;
    }
    delay(50);
  }
}

void init_sideled() {
  // Init default values for leds
    for(int a = 0; a < SIDELED_NUM_LEDS; a++) {
      leds_color[a] = CRGB::Black;
      leds_current[a] = leds_color[a];
      leds_state[a] = SIDELED_STATE_FADE;
      changed = true;
    }
    // Init FastLED
    FastLED.addLeds<NEOPIXEL, SIDELED_DATA_PIN>(leds_current, SIDELED_NUM_LEDS);
    // Start background task
    xTaskCreate(LEDtask, "LEDTask", 4096, NULL, 10, &myTaskHandle);
    xTaskCreatePinnedToCore(LEDtask, "LEDTask", 4096, NULL, 2, &myTaskHandle, 1);
}

void set_sideled_state(uint8_t led_livingroom, uint8_t led_bedroom,uint8_t led_attic, uint8_t state) {
  block = true;
  if(led_livingroom >= SIDELED_NUM_LEDS || led_bedroom > SIDELED_NUM_LEDS || led_attic > SIDELED_NUM_LEDS || led_livingroom >= led_bedroom >= led_attic) return;
  for(uint8_t a = led_livingroom; a < led_attic; a++) {
    leds_state[a] = state;
  }
  changed = true;
}

void set_sideled_color(uint8_t led_livingroom, uint8_t led_bedroom,uint8_t led_attic, CRGB color) {
  block = true;
  if(led_livingroom >= SIDELED_NUM_LEDS || led_bedroom > SIDELED_NUM_LEDS || led_attic > SIDELED_NUM_LEDS || led_livingroom >= led_bedroom >= led_attic) return;
  for(uint8_t a = led_livingroom; a < led_attic; a++) {
    leds_color[a] = color;
  }
  changed = true;
}