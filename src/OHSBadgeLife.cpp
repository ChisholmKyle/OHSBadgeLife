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

// Pin Definitions
// ===============

#define MODE_BUTTON_PIN 26

#define LED_PIN_HEAD_RIGHT 23
#define LED_PIN_HEAD_TOP 4
#define LED_PIN_HEAD_LEFT 3

#define LED_PIN_EYE_RIGHT 19
#define LED_PIN_EYE_LEFT 15

#define LED_PIN_BODY_RIGHT 18
#define LED_PIN_BODY_CENTER 17
#define LED_PIN_BODY_LEFT 16

#define COLOR_PIN_RED 2
#define COLOR_PIN_GREEN 1
#define COLOR_PIN_BLUE 0

// LED Index
// =========

#define LED_HEAD_RIGHT 0
#define LED_HEAD_TOP 1
#define LED_HEAD_LEFT 2

#define LED_EYE_LEFT 3
#define LED_EYE_RIGHT 4

#define LED_BODY_RIGHT 5
#define LED_BODY_CENTER 6
#define LED_BODY_LEFT 7

#define NUM_LEDS 8

// LED functions
// =============

void SetColor(int red, int green, int blue);
void SetColorBrightness(int red, int green, int blue, int brightness);

void TurnOnLed(int index);
void TurnOffLed(int index);

void TurnOnHeadLEDs();
void TurnOffHeadLEDs();

void TurnOnEyeLEDs();
void TurnOffEyeLEDs();

void TurnOnBodyLEDs();
void TurnOffBodyLEDs();

// Animations
// ==========

// Animation
#define ANIM_NUM_MODES 3
int anim_mode = 0;

// button debounce
#define DEBOUNCE_MAX_STEPS 10
bool ButtonDebounce(int button_input);

// polling timer at 20 milliseconds (50 times per second)
#define TIMER_STEP_MS 20
uint32_t timer_prev;

void setup() {
    // PWM cathodes to HIGH
    pinMode(COLOR_PIN_RED, OUTPUT);
    digitalWrite(COLOR_PIN_RED, HIGH);
    pinMode(COLOR_PIN_GREEN, OUTPUT);
    digitalWrite(COLOR_PIN_GREEN, HIGH);
    pinMode(COLOR_PIN_BLUE, OUTPUT);
    digitalWrite(COLOR_PIN_BLUE, HIGH);

    // Anodes to LOW
    pinMode(LED_PIN_HEAD_RIGHT, OUTPUT);
    digitalWrite(LED_PIN_HEAD_RIGHT, LOW);
    pinMode(LED_PIN_HEAD_TOP, OUTPUT);
    digitalWrite(LED_PIN_HEAD_TOP, LOW);
    pinMode(LED_PIN_HEAD_LEFT, OUTPUT);
    digitalWrite(LED_PIN_HEAD_LEFT, LOW);

    pinMode(LED_PIN_EYE_RIGHT, OUTPUT);
    digitalWrite(LED_PIN_EYE_RIGHT, LOW);
    pinMode(LED_PIN_EYE_LEFT, OUTPUT);
    digitalWrite(LED_PIN_EYE_LEFT, LOW);

    pinMode(LED_PIN_BODY_RIGHT, OUTPUT);
    digitalWrite(LED_PIN_BODY_RIGHT, LOW);
    pinMode(LED_PIN_BODY_CENTER, OUTPUT);
    digitalWrite(LED_PIN_BODY_CENTER, LOW);
    pinMode(LED_PIN_BODY_LEFT, OUTPUT);
    digitalWrite(LED_PIN_BODY_LEFT, LOW);

    // Button mode
    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);

    // Start with eyes on
    TurnOffBodyLEDs();
    TurnOffHeadLEDs();
    TurnOnEyeLEDs();

    // Start with green
    SetColor(0, 200, 50);

    // timer begin
    timer_prev = millis();
}

void loop() {
    // get mode button input
    bool button_pressed = ButtonDebounce(digitalRead(MODE_BUTTON_PIN));

    // run timer
    uint32_t now = millis();
    uint32_t elapsed = now - timer_prev;
    if (elapsed >= TIMER_STEP_MS) {
        // advance timer
        timer_prev = timer_prev + TIMER_STEP_MS;

        // process mode button press
        if (button_pressed) {
            // button pressed: go to next animation mode
            anim_mode = (anim_mode + 1) % ANIM_NUM_MODES;
            // transition state
            switch (anim_mode) {
                case 0:
                    // Transition to Eyes
                    TurnOffBodyLEDs();
                    TurnOffHeadLEDs();
                    TurnOnEyeLEDs();
                    break;

                case 1:
                    // Transition to Body
                    TurnOffEyeLEDs();
                    TurnOffHeadLEDs();
                    TurnOnBodyLEDs();
                    break;

                case 2:
                    // Transition to Head
                    TurnOffBodyLEDs();
                    TurnOffEyeLEDs();
                    TurnOnHeadLEDs();
                    break;

                default:
                    break;
            }
        }

        // TODO: animate color
    }
}

void SetColorBrightness(int red, int green, int blue, int brightness) {
    red = (constrain(red, 0, 255) * constrain(brightness, 0, 255)) / 255;
    green = (constrain(green, 0, 255) * constrain(brightness, 0, 255)) / 255;
    blue = (constrain(blue, 0, 255) * constrain(brightness, 0, 255)) / 255;
    analogWrite(COLOR_PIN_RED, red);
    analogWrite(COLOR_PIN_GREEN, green);
    analogWrite(COLOR_PIN_BLUE, blue);
}

void SetColor(int red, int green, int blue) {
    red = 255 - constrain(red, 0, 255);
    green = 255 - constrain(green, 0, 255);
    blue = 255 - constrain(blue, 0, 255);
    analogWrite(COLOR_PIN_RED, red);
    analogWrite(COLOR_PIN_GREEN, green);
    analogWrite(COLOR_PIN_BLUE, blue);
}

void TurnOnLed(int index) {
    switch (index) {
        case LED_HEAD_RIGHT:
            digitalWrite(LED_PIN_HEAD_RIGHT, HIGH);
            break;
        case LED_HEAD_TOP:
            digitalWrite(LED_PIN_HEAD_TOP, HIGH);
            break;
        case LED_HEAD_LEFT:
            digitalWrite(LED_PIN_HEAD_LEFT, HIGH);
            break;
        case LED_EYE_RIGHT:
            digitalWrite(LED_PIN_EYE_RIGHT, HIGH);
            break;
        case LED_EYE_LEFT:
            digitalWrite(LED_PIN_EYE_LEFT, HIGH);
            break;
        case LED_BODY_RIGHT:
            digitalWrite(LED_PIN_BODY_RIGHT, HIGH);
            break;
        case LED_BODY_CENTER:
            digitalWrite(LED_PIN_BODY_CENTER, HIGH);
            break;
        case LED_BODY_LEFT:
            digitalWrite(LED_PIN_BODY_LEFT, HIGH);
            break;
        default:
            break;
    }
}

void TurnOffLed(int index) {
    switch (index) {
        case LED_HEAD_RIGHT:
            digitalWrite(LED_PIN_HEAD_RIGHT, LOW);
            break;
        case LED_HEAD_TOP:
            digitalWrite(LED_PIN_HEAD_TOP, LOW);
            break;
        case LED_HEAD_LEFT:
            digitalWrite(LED_PIN_HEAD_LEFT, LOW);
            break;
        case LED_EYE_RIGHT:
            digitalWrite(LED_PIN_EYE_RIGHT, LOW);
            break;
        case LED_EYE_LEFT:
            digitalWrite(LED_PIN_EYE_LEFT, LOW);
            break;
        case LED_BODY_RIGHT:
            digitalWrite(LED_PIN_BODY_RIGHT, LOW);
            break;
        case LED_BODY_CENTER:
            digitalWrite(LED_PIN_BODY_CENTER, LOW);
            break;
        case LED_BODY_LEFT:
            digitalWrite(LED_PIN_BODY_LEFT, LOW);
            break;
        default:
            break;
    }
}

void TurnOnHeadLEDs() {
    digitalWrite(LED_PIN_HEAD_RIGHT, HIGH);
    digitalWrite(LED_PIN_HEAD_TOP, HIGH);
    digitalWrite(LED_PIN_HEAD_LEFT, HIGH);
}

void TurnOffHeadLEDs() {
    digitalWrite(LED_PIN_HEAD_RIGHT, LOW);
    digitalWrite(LED_PIN_HEAD_TOP, LOW);
    digitalWrite(LED_PIN_HEAD_LEFT, LOW);
}

void TurnOnEyeLEDs() {
    digitalWrite(LED_PIN_EYE_RIGHT, HIGH);
    digitalWrite(LED_PIN_EYE_LEFT, HIGH);
}
void TurnOffEyeLEDs() {
    digitalWrite(LED_PIN_EYE_RIGHT, LOW);
    digitalWrite(LED_PIN_EYE_LEFT, LOW);
}

void TurnOnBodyLEDs() {
    digitalWrite(LED_PIN_BODY_RIGHT, HIGH);
    digitalWrite(LED_PIN_BODY_CENTER, HIGH);
    digitalWrite(LED_PIN_BODY_LEFT, HIGH);
}
void TurnOffBodyLEDs() {
    digitalWrite(LED_PIN_BODY_RIGHT, LOW);
    digitalWrite(LED_PIN_BODY_CENTER, LOW);
    digitalWrite(LED_PIN_BODY_LEFT, LOW);
}

bool ButtonDebounce(int button_input) {
    static int button_prev = HIGH;
    static int button_steps = 0;

    bool button_pressed = false;
    if (button_prev == HIGH) {
        if (button_input == LOW) {
            button_steps++;
            if (button_steps > DEBOUNCE_MAX_STEPS) {
                button_pressed = true;
                button_prev = LOW;
                button_steps = 0;
            }
        } else if (button_steps > 0) {
            button_steps--;
        }
    } else {
        if (button_input == HIGH) {
            button_steps++;
            if (button_steps > DEBOUNCE_MAX_STEPS) {
                button_prev = HIGH;
                button_steps = 0;
            }
        } else if (button_steps > 0) {
            button_steps--;
        }
    }

    return button_pressed;
}