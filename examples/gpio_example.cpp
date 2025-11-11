/*
 * gpio_example.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

/**
 * GPIO Example
 *
 * This example demonstrates how to use the GPIO class from ApraUtils to:
 * 1. Initialize GPIO pins for input and output
 * 2. Read and write digital values
 * 3. Set up edge interrupt detection
 * 4. Handle GPIO errors properly
 *
 * Hardware Setup:
 * - LED connected to GPIO pin 23 (output)
 * - Button connected to GPIO pin 24 (input with pull-up)
 * - Sensor connected to GPIO pin 25 (input with interrupt on rising edge)
 *
 * Compilation:
 * g++ -std=c++14 gpio_example.cpp -I../includes -L../build -lApraLinuxUtils -lpthread -o gpio_example
 *
 * Execution:
 * sudo ./gpio_example
 * (sudo required for GPIO access on most systems)
 */

#include <ApraUtils.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <signal.h>

using namespace apra;
using namespace std;

// Global flag for clean shutdown
volatile bool running = true;

void signalHandler(int signum)
{
    cout << "\nInterrupt signal (" << signum << ") received. Shutting down..." << endl;
    running = false;
}

/**
 * Example 1: Basic Output - Blinking LED
 */
void example_basic_output()
{
    cout << "\n=== Example 1: Basic Output (Blinking LED) ===" << endl;

    // Create GPIO object for pin 23
    GPIO led(23);

    // Initialize as output
    if (!led.Init(false))
    {
        cerr << "Error: Failed to initialize GPIO 23 as output" << endl;
        return;
    }

    cout << "Blinking LED on GPIO 23 for 10 seconds..." << endl;

    // Blink LED for 10 seconds
    for (int i = 0; i < 10 && running; i++)
    {
        led.SetValue(true);   // Turn on
        cout << "LED ON" << endl;
        sleep(1);

        led.SetValue(false);  // Turn off
        cout << "LED OFF" << endl;
        sleep(1);
    }

    // Clean up
    led.UnInit();
    cout << "LED example completed." << endl;
}

/**
 * Example 2: Basic Input - Reading Button State
 */
void example_basic_input()
{
    cout << "\n=== Example 2: Basic Input (Reading Button) ===" << endl;

    // Create GPIO object for pin 24
    GPIO button(24);

    // Initialize as input
    if (!button.Init(true))
    {
        cerr << "Error: Failed to initialize GPIO 24 as input" << endl;
        return;
    }

    cout << "Reading button state on GPIO 24 for 10 seconds..." << endl;
    cout << "Press the button to see the state change." << endl;

    // Read button state for 10 seconds
    for (int i = 0; i < 100 && running; i++)
    {
        bool buttonState = button.GetValue();
        cout << "Button state: " << (buttonState ? "PRESSED" : "RELEASED") << endl;
        usleep(100000);  // 100ms delay
    }

    // Clean up
    button.UnInit();
    cout << "Button example completed." << endl;
}

/**
 * Example 3: Edge Interrupt Detection
 */
void example_edge_interrupt()
{
    cout << "\n=== Example 3: Edge Interrupt Detection ===" << endl;

    // Create GPIO object for pin 25
    GPIO sensor(25);

    // Initialize for edge interrupt detection (rising edge)
    if (!sensor.Init4EdgeInterrupt(true, RISING))
    {
        cerr << "Error: Failed to initialize GPIO 25 for interrupt" << endl;
        return;
    }

    cout << "Waiting for rising edge events on GPIO 25 for 30 seconds..." << endl;
    cout << "Trigger the sensor to generate events." << endl;

    int eventCount = 0;
    time_t startTime = time(NULL);

    while (running && (time(NULL) - startTime) < 30)
    {
        // Wait for edge event with 1 second timeout
        if (sensor.WaitForEdge(1000000))  // 1 second = 1,000,000 microseconds
        {
            eventCount++;
            cout << "Event #" << eventCount << " detected at GPIO 25!" << endl;

            // Read the current value
            bool value = sensor.GetValue();
            cout << "Current value: " << (value ? "HIGH" : "LOW") << endl;
        }
    }

    cout << "Total events detected: " << eventCount << endl;

    // Clean up
    sensor.UnInit();
    cout << "Interrupt example completed." << endl;
}

/**
 * Example 4: Combined Input/Output - Button Controlled LED
 */
void example_button_controlled_led()
{
    cout << "\n=== Example 4: Button Controlled LED ===" << endl;

    // Setup LED (output)
    GPIO led(23);
    if (!led.Init(false))
    {
        cerr << "Error: Failed to initialize LED GPIO" << endl;
        return;
    }

    // Setup button (input)
    GPIO button(24);
    if (!button.Init(true))
    {
        cerr << "Error: Failed to initialize button GPIO" << endl;
        led.UnInit();
        return;
    }

    cout << "LED on GPIO 23 will follow button state on GPIO 24" << endl;
    cout << "Running for 20 seconds..." << endl;

    time_t startTime = time(NULL);

    while (running && (time(NULL) - startTime) < 20)
    {
        // Read button state
        bool buttonPressed = button.GetValue();

        // Set LED to match button state
        led.SetValue(buttonPressed);

        // Small delay
        usleep(50000);  // 50ms
    }

    // Clean up
    led.UnInit();
    button.UnInit();
    cout << "Button-controlled LED example completed." << endl;
}

/**
 * Main function - Run all examples
 */
int main(int argc, char** argv)
{
    // Setup signal handler for clean shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    cout << "ApraLinuxUtils GPIO Examples" << endl;
    cout << "=======================" << endl;
    cout << "\nThese examples demonstrate GPIO functionality." << endl;
    cout << "Make sure you have the necessary hardware connected." << endl;
    cout << "\nPress Ctrl+C to stop any example early.\n" << endl;

    // Check command line arguments
    if (argc > 1)
    {
        string example = argv[1];

        if (example == "output" || example == "1")
        {
            example_basic_output();
        }
        else if (example == "input" || example == "2")
        {
            example_basic_input();
        }
        else if (example == "interrupt" || example == "3")
        {
            example_edge_interrupt();
        }
        else if (example == "combined" || example == "4")
        {
            example_button_controlled_led();
        }
        else
        {
            cout << "Usage: " << argv[0] << " [output|input|interrupt|combined|1|2|3|4]" << endl;
            return 1;
        }
    }
    else
    {
        // Run all examples sequentially
        example_basic_output();

        if (running) example_basic_input();
        if (running) example_edge_interrupt();
        if (running) example_button_controlled_led();
    }

    cout << "\nAll GPIO examples completed successfully!" << endl;

    return 0;
}
