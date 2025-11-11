/*
 * pwm_example.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

/*
 * PWM Control Example
 *
 * This example demonstrates how to use the ApraLinuxUtils PWM interface to:
 * 1. Control LED brightness by varying duty cycle
 * 2. Control servo motor position
 * 3. Demonstrate different PWM frequencies
 * 4. Handle PWM setup and cleanup properly
 *
 * Hardware Requirements:
 * ---------------------
 * - Linux-based embedded system with PWM support (e.g., Raspberry Pi, BeagleBone)
 * - LED with current-limiting resistor (220-330 ohm) OR
 * - Standard servo motor (e.g., SG90, MG995)
 * - External power supply for servo (5V recommended)
 *
 * Wiring for LED:
 * ---------------
 * PWM Output -> LED Anode (+)
 * LED Cathode (-) -> 330 ohm resistor -> Ground
 *
 * Wiring for Servo:
 * -----------------
 * Servo Signal (usually yellow/white) -> PWM Output
 * Servo VCC (red) -> 5V external power supply
 * Servo GND (brown/black) -> Common ground with board
 *
 * PWM System Path:
 * ----------------
 * Linux PWM is exposed through sysfs at /sys/class/pwm/
 * Format: /sys/class/pwm/pwmchip{X}/pwm{Y}/
 * where X is the chip number and Y is the PWM channel number
 *
 * Compilation:
 * ------------
 * g++ -std=c++14 pwm_example.cpp -o pwm_example -lApraLinuxUtils
 *
 * Execution:
 * ----------
 * sudo ./pwm_example
 * (sudo is required for PWM sysfs access)
 *
 * Note: Modify PWM_CHIP and PWM_PIN for your specific hardware
 */

#include <ApraUtils.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace apra;
using namespace std;

// PWM Configuration
// For Raspberry Pi: typically pwmchip0, pins 0 or 1
// For BeagleBone: pwmchip numbers vary (check /sys/class/pwm/)
#define PWM_CHIP 0
#define PWM_PIN 0

// PWM Timing Constants (in nanoseconds)
#define LED_PWM_PERIOD_NS 20000000ULL        // 20ms = 50Hz (common for LED dimming)
#define SERVO_PWM_PERIOD_NS 20000000ULL      // 20ms = 50Hz (standard for servos)
#define SERVO_MIN_PULSE_NS 1000000ULL        // 1ms pulse = 0 degrees
#define SERVO_MAX_PULSE_NS 2000000ULL        // 2ms pulse = 180 degrees
#define SERVO_CENTER_PULSE_NS 1500000ULL     // 1.5ms pulse = 90 degrees

// Control flag
volatile bool g_keepRunning = true;

// Signal handler for graceful shutdown
void signalHandler(int signum) {
    cout << "\nInterrupt signal (" << signum << ") received. Shutting down..." << endl;
    g_keepRunning = false;
}

// Function to demonstrate LED brightness control
void ledBrightnessExample() {
    cout << "\n==================================================" << endl;
    cout << "LED Brightness Control Example" << endl;
    cout << "==================================================" << endl;

    // Create PWM instance
    PWM ledPWM(PWM_CHIP, PWM_PIN, true);

    // Setup PWM with 50Hz frequency and 0% initial duty cycle
    if (!ledPWM.setup(LED_PWM_PERIOD_NS, 0)) {
        cerr << "Failed to setup PWM for LED." << endl;
        cerr << "Check if PWM chip and pin are correct." << endl;
        return;
    }

    cout << "PWM setup successful for LED." << endl;
    cout << "Period: 20ms (50Hz)" << endl;
    cout << "Initial duty cycle: 0%" << endl;

    // Start PWM
    if (!ledPWM.start()) {
        cerr << "Failed to start PWM." << endl;
        ledPWM.destroy();
        return;
    }

    cout << "PWM started." << endl;
    cout << "\nDemonstrating LED fade-in and fade-out..." << endl;

    // Fade in: 0% to 100%
    cout << "\nFading in (0% -> 100%):" << endl;
    for (uint32_t brightness = 0; brightness <= 100 && g_keepRunning; brightness += 5) {
        ledPWM.changeDutyCycle(brightness);
        cout << "  Brightness: " << brightness << "%" << endl;
        usleep(200000); // 200ms delay
    }

    sleep(1);

    // Fade out: 100% to 0%
    cout << "\nFading out (100% -> 0%):" << endl;
    for (int32_t brightness = 100; brightness >= 0 && g_keepRunning; brightness -= 5) {
        ledPWM.changeDutyCycle(brightness);
        cout << "  Brightness: " << brightness << "%" << endl;
        usleep(200000); // 200ms delay
    }

    sleep(1);

    // Demonstrate specific brightness levels
    cout << "\nDemonstrating specific brightness levels:" << endl;
    uint32_t levels[] = {25, 50, 75, 100};
    for (int i = 0; i < 4 && g_keepRunning; i++) {
        cout << "  Setting brightness to " << levels[i] << "%" << endl;
        ledPWM.changeDutyCycle(levels[i]);
        sleep(2);
    }

    // Cleanup
    cout << "\nStopping LED PWM..." << endl;
    ledPWM.stop();
    ledPWM.destroy();
    cout << "LED example completed." << endl;
}

// Function to convert servo angle to pulse width in nanoseconds
uint64_t angleToNanoseconds(int angle) {
    // Clamp angle to 0-180 degrees
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // Linear interpolation: 0° = 1ms, 180° = 2ms
    return SERVO_MIN_PULSE_NS +
           ((SERVO_MAX_PULSE_NS - SERVO_MIN_PULSE_NS) * angle / 180);
}

// Function to demonstrate servo motor control
void servoControlExample() {
    cout << "\n==================================================" << endl;
    cout << "Servo Motor Control Example" << endl;
    cout << "==================================================" << endl;

    // Create PWM instance
    PWM servoPWM(PWM_CHIP, PWM_PIN, true);

    // Setup PWM with 50Hz frequency (20ms period)
    // Initial position: 90 degrees (center)
    uint64_t initialPulse = SERVO_CENTER_PULSE_NS;

    if (!servoPWM.setup(SERVO_PWM_PERIOD_NS, initialPulse)) {
        cerr << "Failed to setup PWM for servo." << endl;
        return;
    }

    cout << "PWM setup successful for servo." << endl;
    cout << "Period: 20ms (50Hz)" << endl;
    cout << "Initial position: 90 degrees (center)" << endl;

    // Start PWM
    if (!servoPWM.start()) {
        cerr << "Failed to start PWM." << endl;
        servoPWM.destroy();
        return;
    }

    cout << "PWM started." << endl;
    sleep(1);

    // Move servo to predefined positions
    cout << "\nMoving servo to specific angles:" << endl;

    int angles[] = {0, 45, 90, 135, 180, 90};
    const char* positions[] = {"0° (Far left)", "45° (Left)", "90° (Center)",
                               "135° (Right)", "180° (Far right)", "90° (Center)"};

    for (int i = 0; i < 6 && g_keepRunning; i++) {
        uint64_t pulseWidth = angleToNanoseconds(angles[i]);
        cout << "  Moving to " << positions[i] << endl;
        cout << "    Pulse width: " << pulseWidth / 1000 << " microseconds" << endl;

        servoPWM.updateDutyCycle(pulseWidth);
        sleep(2); // Give servo time to move
    }

    // Smooth sweep demonstration
    if (g_keepRunning) {
        cout << "\nPerforming smooth sweep (0° -> 180°):" << endl;
        for (int angle = 0; angle <= 180 && g_keepRunning; angle += 5) {
            uint64_t pulseWidth = angleToNanoseconds(angle);
            servoPWM.updateDutyCycle(pulseWidth);
            cout << "  Angle: " << angle << "°" << endl;
            usleep(100000); // 100ms between steps
        }

        sleep(1);

        cout << "\nPerforming smooth sweep (180° -> 0°):" << endl;
        for (int angle = 180; angle >= 0 && g_keepRunning; angle -= 5) {
            uint64_t pulseWidth = angleToNanoseconds(angle);
            servoPWM.updateDutyCycle(pulseWidth);
            cout << "  Angle: " << angle << "°" << endl;
            usleep(100000); // 100ms between steps
        }
    }

    // Return to center
    cout << "\nReturning to center position (90°)..." << endl;
    servoPWM.updateDutyCycle(SERVO_CENTER_PULSE_NS);
    sleep(1);

    // Cleanup
    cout << "\nStopping servo PWM..." << endl;
    servoPWM.stop();
    servoPWM.destroy();
    cout << "Servo example completed." << endl;
}

// Function to demonstrate frequency variations
void frequencyVariationExample() {
    cout << "\n==================================================" << endl;
    cout << "PWM Frequency Variation Example" << endl;
    cout << "==================================================" << endl;

    PWM varPWM(PWM_CHIP, PWM_PIN, true);

    // Test different frequencies with 50% duty cycle
    struct FrequencyTest {
        uint64_t periodNs;
        const char* description;
    };

    FrequencyTest tests[] = {
        {20000000, "50Hz (20ms) - Standard servo frequency"},
        {10000000, "100Hz (10ms) - Fast PWM"},
        {1000000,  "1kHz (1ms) - High frequency"},
        {100000,   "10kHz (100us) - Very high frequency"}
    };

    for (int i = 0; i < 4 && g_keepRunning; i++) {
        cout << "\nTesting: " << tests[i].description << endl;

        uint64_t dutyCycle = tests[i].periodNs / 2; // 50% duty cycle

        if (!varPWM.setup(tests[i].periodNs, dutyCycle)) {
            cerr << "Failed to setup PWM with period " << tests[i].periodNs << "ns" << endl;
            continue;
        }

        if (!varPWM.start()) {
            cerr << "Failed to start PWM" << endl;
            varPWM.destroy();
            continue;
        }

        cout << "  Period: " << tests[i].periodNs / 1000 << " microseconds" << endl;
        cout << "  Duty cycle: " << varPWM.getDutyCyclePercent() << "%" << endl;
        cout << "  Running for 3 seconds..." << endl;

        sleep(3);

        varPWM.stop();
        varPWM.destroy();
        sleep(1);
    }

    cout << "\nFrequency variation example completed." << endl;
}

// Main function
int main() {
    cout << "==================================================" << endl;
    cout << "ApraLinuxUtils PWM Control Example" << endl;
    cout << "==================================================" << endl;
    cout << "This example demonstrates PWM control for:" << endl;
    cout << "  1. LED brightness control" << endl;
    cout << "  2. Servo motor positioning" << endl;
    cout << "  3. Different PWM frequencies" << endl;
    cout << "\nMake sure you have connected the appropriate hardware." << endl;
    cout << "Press Ctrl+C to stop at any time." << endl;
    cout << "==================================================" << endl;

    // Setup signal handler
    signal(SIGINT, signalHandler);

    // Wait for user to be ready
    cout << "\nPress Enter to start the examples...";
    cin.get();

    // Run examples
    if (g_keepRunning) {
        ledBrightnessExample();
    }

    if (g_keepRunning) {
        sleep(2);
        cout << "\nPress Enter to continue to servo example...";
        cin.get();
        servoControlExample();
    }

    if (g_keepRunning) {
        sleep(2);
        cout << "\nPress Enter to continue to frequency variation example...";
        cin.get();
        frequencyVariationExample();
    }

    cout << "\n==================================================" << endl;
    cout << "All PWM examples completed successfully." << endl;
    cout << "==================================================" << endl;

    return 0;
}
