/*
  Open Hardware Summit 2024
  Designed by Cyber City Circuits
  Twitter @MakeAugusta

  This board uses Minicore with the Arduino IDE:
  https://github.com/MCUdude/MiniCore?tab=readme-ov-file#how-to-install

  Board: ATMEGA328
  Clock: Internal 8MHz
  Variant: 328PB
  Port: Comm Port used for your programmer
  Programmer: Arduino as ISP

  Pin LEDs:
  Anode 1: 23 - Head Ring Right
  Anode 2: 04 - Head Ring Top
  Anode 3: 03 - Head Ring Left
  Anode 4: 19 - Eye Right
  Anode 5: 18 - Body Right
  Anode 6: 17 - Body Center
  Anode 7: 16 - Body Left
  Anode 8: 15 - Eye Left

  Pin RGB for LED 1-4:
  Blue:  00
  Green: 01
  Red:   02

  Button: 26 (Active Low)

  SAO GPIO:
  SAO 1: 06
  SAO 2: 05

*/
#include <Arduino.h>
#include <FastLED.h>

#include "Debounce.h"
#include "OHS2024Badge.h"

// badge LED control
OHS2024Badge badge = {};

// mode button press with debounce
Debounce debounce = {};
bool button_press_processed = false;
const byte mode_button_pin = 26;

// Animation
const byte anim_num_modes = 3;
const unsigned long anim_speed_period_ms = 1000;
unsigned long anim_time = 0;
byte anim_mode = 0;

// colors
const CRGB color_start = CRGB(0, 0, 0);
const CRGB color_eyes = CRGB(0, 200, 100);
const CRGB color_body = CRGB(200, 100, 0);
const CRGB color_head = CRGB(100, 0, 200);
CRGB color_current = color_eyes;

// polling timer at 20 milliseconds (50 times per second)
const unsigned long timer_step = 20;
unsigned long timer_prev;

void setup() {
    // initialize badge LEDs
    badge.Setup();

    // Button mode
    pinMode(mode_button_pin, INPUT_PULLUP);
    DebounceConfiguration debounce_config = {};
    debounce_config.pin = mode_button_pin;
    debounce_config.polarity = HIGH;
    debounce_config.max_count = 20;
    debounce_config.delay_microseconds = 5000;
    debounce.Setup(debounce_config);

    // initial animation
    anim_mode = 0;
    anim_time = 0;

    // Start with eyes on
    badge.TurnOffBodyLEDs();
    badge.TurnOffHeadLEDs();
    badge.TurnOnEyeLEDs();

    // timer begin
    timer_prev = millis();
}

void loop() {
    const unsigned long now = millis();
    const byte mode_reading = digitalRead(mode_button_pin);

    // Get button press state
    const bool button_press = debounce.Update();
    if ((!button_press) && button_press_processed)
    {
        // reset when button is released
        button_press_processed = false;
    }

    // run timer
    const unsigned long elapsed = now - timer_prev;
    if (elapsed >= timer_step) {
        // advance timer
        timer_prev = timer_prev + timer_step;

        // process mode button press
        if (button_press && !button_press_processed) {
            button_press_processed = true;
            // button pressed: go to next animation mode
            anim_mode = (anim_mode + 1) % anim_mode;
            // transition state
            switch (anim_mode) {
                case 0:
                    // Transition to Eyes
                    badge.TurnOffBodyLEDs();
                    badge.TurnOffHeadLEDs();
                    badge.TurnOnEyeLEDs();
                    color_current = color_eyes;
                    break;

                case 1:
                    // Transition to Body
                    badge.TurnOffEyeLEDs();
                    badge.TurnOffHeadLEDs();
                    badge.TurnOnBodyLEDs();
                    color_current = color_body;
                    break;

                case 2:
                    // Transition to Head
                    badge.TurnOffBodyLEDs();
                    badge.TurnOffEyeLEDs();
                    badge.TurnOnHeadLEDs();
                    color_current = color_head;
                    break;

                default:
                    break;
            }
        }

        // color animation
        anim_time = anim_time + timer_step;
        if (anim_time >= anim_speed_period_ms) {
            // wrap animation time when exceeds period
            anim_time = (anim_time - anim_speed_period_ms);
        }
        // blend with breathing waveform
        const byte wave_input = static_cast<byte>((anim_time * UINT8_MAX) / anim_speed_period_ms);
        const byte blend_amount = quadwave8(wave_input);
        // blend color
        const CRGB color = blend(color_start, color_current, blend_amount);
        badge.SetColor(color.r, color.g, color.b);
    }
}
