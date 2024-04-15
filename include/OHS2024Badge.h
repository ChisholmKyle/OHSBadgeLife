

#include <Arduino.h>

enum class OHS2024BadgeLED
{
    HeadRight = 0,
    HeadTop,
    HeadLeft,
    EyeRight,
    EyeLeft,
    BodyRight,
    BodyCenter,
    BodyLeft,
    NumLEDs
};

struct OHS2024BadgePins
{
    byte head_right = 23;
    byte head_top = 4;
    byte head_left = 3;

    byte eye_right = 19;
    byte eye_left = 15;

    byte body_right = 18;
    byte body_center = 17;
    byte body_left = 16;

    byte pwm_red = 2;
    byte pwm_green = 1;
    byte pwm_blue = 0;
};

class OHS2024Badge
{
public:
    OHS2024Badge() = default;

    inline void Setup();

    inline void SetColor(byte red, byte green, byte blue);

    inline void TurnOnLED(OHS2024BadgeLED led);
    inline void TurnOffLED(OHS2024BadgeLED led);

    inline void TurnOnHeadLEDs();
    inline void TurnOffHeadLEDs();

    inline void TurnOnEyeLEDs();
    inline void TurnOffEyeLEDs();

    inline void TurnOnBodyLEDs();
    inline void TurnOffBodyLEDs();

private:
    OHS2024BadgePins m_pins = OHS2024BadgePins();
};

void OHS2024Badge::Setup()
{
    // PWM cathodes to HIGH
    pinMode(m_pins.pwm_red, OUTPUT);
    digitalWrite(m_pins.pwm_red, HIGH);
    pinMode(m_pins.pwm_green, OUTPUT);
    digitalWrite(m_pins.pwm_green, HIGH);
    pinMode(m_pins.pwm_blue, OUTPUT);
    digitalWrite(m_pins.pwm_blue, HIGH);

    // Anodes to LOW
    pinMode(m_pins.head_right, OUTPUT);
    digitalWrite(m_pins.head_right, LOW);
    pinMode(m_pins.head_top, OUTPUT);
    digitalWrite(m_pins.head_top, LOW);
    pinMode(m_pins.head_left, OUTPUT);
    digitalWrite(m_pins.head_left, LOW);

    pinMode(m_pins.eye_right, OUTPUT);
    digitalWrite(m_pins.eye_right, LOW);
    pinMode(m_pins.eye_left, OUTPUT);
    digitalWrite(m_pins.eye_left, LOW);

    pinMode(m_pins.body_right, OUTPUT);
    digitalWrite(m_pins.body_right, LOW);
    pinMode(m_pins.body_center, OUTPUT);
    digitalWrite(m_pins.body_center, LOW);
    pinMode(m_pins.body_left, OUTPUT);
    digitalWrite(m_pins.body_left, LOW);
}

void OHS2024Badge::SetColor(byte red, byte green, byte blue)
{
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
    analogWrite(m_pins.pwm_red, red);
    analogWrite(m_pins.pwm_green, green);
    analogWrite(m_pins.pwm_blue, blue);
}

void OHS2024Badge::TurnOnLED(const OHS2024BadgeLED led)
{
    switch (led) {
        case OHS2024BadgeLED::HeadRight:
            digitalWrite(m_pins.head_right, HIGH);
            break;
        case OHS2024BadgeLED::HeadTop:
            digitalWrite(m_pins.head_top, HIGH);
            break;
        case OHS2024BadgeLED::HeadLeft:
            digitalWrite(m_pins.head_left, HIGH);
            break;
        case OHS2024BadgeLED::EyeRight:
            digitalWrite(m_pins.eye_right, HIGH);
            break;
        case OHS2024BadgeLED::EyeLeft:
            digitalWrite(m_pins.eye_left, HIGH);
            break;
        case OHS2024BadgeLED::BodyRight:
            digitalWrite(m_pins.body_right, HIGH);
            break;
        case OHS2024BadgeLED::BodyCenter:
            digitalWrite(m_pins.body_center, HIGH);
            break;
        case OHS2024BadgeLED::BodyLeft:
            digitalWrite(m_pins.body_left, HIGH);
            break;
        default:
            break;
    }
}

void OHS2024Badge::TurnOffLED(const OHS2024BadgeLED led)
{
    switch (led) {
        case OHS2024BadgeLED::HeadRight:
            digitalWrite(m_pins.head_right, LOW);
            break;
        case OHS2024BadgeLED::HeadTop:
            digitalWrite(m_pins.head_top, LOW);
            break;
        case OHS2024BadgeLED::HeadLeft:
            digitalWrite(m_pins.head_left, LOW);
            break;
        case OHS2024BadgeLED::EyeRight:
            digitalWrite(m_pins.eye_right, LOW);
            break;
        case OHS2024BadgeLED::EyeLeft:
            digitalWrite(m_pins.eye_left, LOW);
            break;
        case OHS2024BadgeLED::BodyRight:
            digitalWrite(m_pins.body_right, LOW);
            break;
        case OHS2024BadgeLED::BodyCenter:
            digitalWrite(m_pins.body_center, LOW);
            break;
        case OHS2024BadgeLED::BodyLeft:
            digitalWrite(m_pins.body_left, LOW);
            break;
        default:
            break;
    }
}

void OHS2024Badge::TurnOnHeadLEDs()
{
    digitalWrite(m_pins.head_right, HIGH);
    digitalWrite(m_pins.head_top, HIGH);
    digitalWrite(m_pins.head_left, HIGH);
}

void OHS2024Badge::TurnOffHeadLEDs()
{
    digitalWrite(m_pins.head_right, LOW);
    digitalWrite(m_pins.head_top, LOW);
    digitalWrite(m_pins.head_left, LOW);
}

void OHS2024Badge::TurnOnEyeLEDs()
{
    digitalWrite(m_pins.eye_right, HIGH);
    digitalWrite(m_pins.eye_left, HIGH);
}
void OHS2024Badge::TurnOffEyeLEDs()
{
    digitalWrite(m_pins.eye_right, LOW);
    digitalWrite(m_pins.eye_left, LOW);
}

void OHS2024Badge::TurnOnBodyLEDs()
{
    digitalWrite(m_pins.body_right, HIGH);
    digitalWrite(m_pins.body_center, HIGH);
    digitalWrite(m_pins.body_left, HIGH);
}
void OHS2024Badge::TurnOffBodyLEDs()
{
    digitalWrite(m_pins.body_right, LOW);
    digitalWrite(m_pins.body_center, LOW);
    digitalWrite(m_pins.body_left, LOW);
}