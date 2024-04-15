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

#include "OHS2024Badge.h"

// badge LED control
OHS2024Badge badge = {};

// aniamtion modes
const byte anim_num_modes = 3;
const unsigned long anim_speed_period_ms = 1000;
unsigned long anim_time = 0;
byte anim_mode = 0;

// mode button press with debounce
const byte mode_button_pin = 26;
// debounce
const byte mode_debounce_delay = 200;
unsigned long mode_debounce_time_prev = 0;
byte mode_button_state = HIGH;
byte mode_button_state_prev = HIGH;

// state of color chosen
CRGB color_start = {};
CRGB color_fade = {};
byte color_fade_state = 0;

// polling timer at 20 milliseconds (50 times per second)
const unsigned long timer_step = 20;
unsigned long timer_prev;

void setup() {
    // initialize badge LEDs
    badge.Setup();

    // Initial mode state
    pinMode(mode_button_pin, INPUT_PULLUP);
    mode_button_state = HIGH;
    mode_button_state_prev = HIGH;

    // initial animation
    anim_mode = 0;
    anim_time = 0;

    // Start with eyes on
    badge.TurnOffBodyLEDs();
    badge.TurnOffHeadLEDs();
    badge.TurnOnEyeLEDs();

    // Initialize start and end color
    color_start = CRGB{0, 0, 0};
    color_fade = CRGB(0, 200, 80);
    color_fade_state = 0;

    // timer begin
    timer_prev = millis();
    // mdoe debounce last time
    mode_debounce_time_prev = timer_prev;
}

void loop() {
    const unsigned long now = millis();
    const byte mode_reading = digitalRead(mode_button_pin);

    // Debounce: button state changed, due to noise or pressing, log time
    if (mode_reading != mode_button_state_prev) {
        mode_debounce_time_prev = now;
    }

    // run timer
    const unsigned long elapsed = now - timer_prev;
    if (elapsed >= timer_step) {
        // advance timer
        timer_prev = timer_prev + timer_step;

        // Check for button press with debounce (Falling edge)
        bool mode_button_pressed = false;
        if (((now - mode_debounce_time_prev) > mode_debounce_delay) && (mode_reading != mode_button_state)) {
            // Enough time has passed without changing state
            mode_button_state = mode_reading;
            // only consider it pressed on falling edge to low state
            mode_button_pressed = (mode_reading == LOW);
        }

        // handle mode button press
        if (mode_button_pressed) {
            // cycle animation modes
            anim_mode = (anim_mode + 1) % anim_mode;
            // transition state
            switch (anim_mode) {
                case 0:
                    // Eyes
                    badge.TurnOffBodyLEDs();
                    badge.TurnOffHeadLEDs();
                    badge.TurnOnEyeLEDs();
                    color_fade = CRGB(0, 200, 100);
                    break;

                case 1:
                    // Body
                    badge.TurnOffEyeLEDs();
                    badge.TurnOffHeadLEDs();
                    badge.TurnOnBodyLEDs();
                    color_fade = CRGB(200, 100, 0);
                    break;

                case 2:
                    // Head
                    badge.TurnOffBodyLEDs();
                    badge.TurnOffEyeLEDs();
                    badge.TurnOnHeadLEDs();
                    color_fade = CRGB(100, 0, 200);
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
        // fade with breathing waveform
        const byte wave_input = static_cast<byte>((anim_time * UINT8_MAX) / anim_speed_period_ms);
        color_fade_state = quadwave8(wave_input);
        // blend color
        const CRGB color = blend(color_start, color_fade, color_fade_state);
        badge.SetColor(color.r, color.g, color.b);
    }
}
