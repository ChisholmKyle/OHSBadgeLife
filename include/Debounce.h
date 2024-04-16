
#pragma once

#include <Arduino.h>

/**
 * @brief Configuration for debounce
 */
struct DebounceConfiguration {
    /**
     * @brief Input pin.
     */
    uint8_t pin = 0;
    /**
     * @brief Pin polarity. Set this to LOW state for regular polarity or HIGH for reversed polarity.
     */
    uint8_t polarity = LOW;
    /**
     * @brief Count of measurements required to switch between states.
     */
    uint16_t max_count = 8;
    /**
     * @brief Minimum delay between each measurement in microseconds.
     */
    uint32_t delay_microseconds = 2000;
};

/**
 * @brief Debounce digital inputs.
 */
class Debounce {
   public:
    Debounce() = default;

    /**
     * @brief Configure debounce parameters.
     *
     * @param config Debounce configuration parameters.
     */
    void Setup(const DebounceConfiguration &config);

    /**
     * @brief Reset state of pin.
     *
     * @param pin_state State of pin.
     */
    void Reset(bool pin_state);

    /**
     * @brief Update debounce and get current state of pin.
     *
     * One measurement will be taken and the state updated. If the counts reaches max values
     * configured for state change, then it will change to the new state.
     *
     * @return Current state of pin.
     */
    bool Update();

   private:
    DebounceConfiguration m_config = {};

    bool m_state = 0;
    uint16_t m_trigger_count = 0;
    uint32_t m_last_update_time = 0;
};

// Inline functions
// ----------------

inline void Debounce::Setup(const DebounceConfiguration &config) {
    m_config = config;
    Reset(false);
}

inline void Debounce::Reset(bool pin_state) {
    m_state = pin_state;
    m_trigger_count = 0;
    m_last_update_time = micros();
}

inline bool Debounce::Update() {
    const uint32_t now = micros();
    if ((now - m_last_update_time) >= m_config.delay_microseconds) {
        // update time
        m_last_update_time = now;
        // take measurement
        bool measured_state = (m_config.polarity != digitalRead(m_config.pin));
        // run debounce
        if (m_state) {
            // state is high, check if low
            if (!measured_state) {
                m_trigger_count++;
                if (m_trigger_count >= m_config.max_count) {
                    // change state
                    m_state = false;
                    m_trigger_count = 0;
                }
            } else if (m_trigger_count > 0) {
                m_trigger_count--;
            }
        } else {
            // state is low, check if high
            if (measured_state) {
                m_trigger_count++;
                if (m_trigger_count >= m_config.max_count) {
                    // change state
                    m_state = true;
                    m_trigger_count = 0;
                }
            } else if (m_trigger_count > 0) {
                m_trigger_count--;
            }
        }
    }
    return m_state;
}
