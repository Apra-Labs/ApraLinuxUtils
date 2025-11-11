/*
 * i2c_example.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

/*
 * I2C Communication Example
 *
 * This example demonstrates how to use the ApraLinuxUtils I2C interface to:
 * 1. Read temperature data from a TMP102 sensor (I2C address 0x48)
 * 2. Write configuration to the sensor
 * 3. Handle I2C transactions with error checking
 * 4. Use asynchronous I2C operations with callbacks
 *
 * Hardware Requirements:
 * ---------------------
 * - Linux-based embedded system with I2C support
 * - TMP102 temperature sensor (or compatible I2C sensor)
 * - I2C bus connected (typically /dev/i2c-1 or /dev/i2c-0)
 * - Pull-up resistors on SDA and SCL lines (typically 4.7K ohms)
 *
 * Wiring (for TMP102):
 * --------------------
 * TMP102 VCC  -> 3.3V
 * TMP102 GND  -> Ground
 * TMP102 SDA  -> I2C SDA (with pull-up)
 * TMP102 SCL  -> I2C SCL (with pull-up)
 * TMP102 ADD0 -> Ground (for address 0x48)
 *
 * Compilation:
 * ------------
 * g++ -std=c++14 i2c_example.cpp -o i2c_example -lApraLinuxUtils -lpthread
 *
 * Execution:
 * ----------
 * sudo ./i2c_example
 * (sudo may be required for I2C device access)
 *
 * Note: Modify I2C_BUS_PATH if your I2C bus is at a different location
 */

#include <ApraUtils.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace apra;
using namespace std;

// I2C Configuration
#define I2C_BUS_PATH "/dev/i2c-1"           // Change to /dev/i2c-0 if needed
#define TMP102_ADDR 0x48                     // TMP102 default address
#define TMP102_TEMP_REG 0x00                 // Temperature register
#define TMP102_CONFIG_REG 0x01               // Configuration register

// Thread control
volatile bool g_keepRunning = true;

// Signal handler for graceful shutdown
void signalHandler(int signum) {
    cout << "\nInterrupt signal (" << signum << ") received. Shutting down..." << endl;
    g_keepRunning = false;
}

// Callback function for asynchronous I2C transactions
void i2cTransactionCallback(I2C_Transaction_Message* transaction, void* context) {
    cout << "\n=== Async Transaction Callback ===" << endl;

    // Check if transaction was successful
    I2CError error = transaction->getError();
    if (error.hasError) {
        cout << "Transaction failed with error code: " << error.errorCode << endl;
        return;
    }

    cout << "Transaction completed successfully!" << endl;

    // Process the response messages
    vector<I2C_Message>& messages = transaction->getAllMessages();
    for (size_t i = 0; i < messages.size(); i++) {
        I2C_Message& msg = messages[i];

        if (msg.m_type == I2C_READ && msg.m_data.size() >= 2) {
            // Convert raw temperature data to Celsius
            // TMP102 format: 12-bit temperature in bits 15-4
            int16_t rawTemp = (msg.m_data[0] << 8) | msg.m_data[1];
            rawTemp >>= 4; // Shift to get 12-bit value

            // Check if negative (bit 11 is sign bit)
            if (rawTemp & 0x800) {
                rawTemp |= 0xF000; // Sign extend
            }

            float temperature = rawTemp * 0.0625; // Each bit = 0.0625°C

            cout << "Temperature: " << temperature << "°C" << endl;
        }
    }
}

// Function to create a temperature read transaction
I2C_Transaction_Message createTempReadTransaction() {
    // Create a read message for temperature register
    I2C_Message readMsg;
    readMsg.configureRead(TMP102_TEMP_REG, 1, 2); // Read 2 bytes from register 0x00
    readMsg.setRetries(3); // Retry up to 3 times on failure

    // Create transaction with the read message
    vector<I2C_Message> messages;
    messages.push_back(readMsg);

    I2C_Transaction_Message transaction(TMP102_ADDR, messages);
    transaction.m_stopOnAnyTransactionFailure = true;

    return transaction;
}

// Function to create a configuration write transaction
I2C_Transaction_Message createConfigWriteTransaction() {
    // TMP102 configuration: continuous conversion, 12-bit resolution
    vector<uint8_t> configData;
    configData.push_back(0x60); // High byte: continuous conversion
    configData.push_back(0xA0); // Low byte: 12-bit resolution

    // Create write message
    I2C_Message writeMsg;
    writeMsg.configureWrite(TMP102_CONFIG_REG, 1, configData, 2);
    writeMsg.setRetries(3);

    // Create transaction
    vector<I2C_Message> messages;
    messages.push_back(writeMsg);

    I2C_Transaction_Message transaction(TMP102_ADDR, messages);
    transaction.m_stopOnAnyTransactionFailure = true;

    return transaction;
}

// Function to perform synchronous I2C read
void synchronousReadExample(I2C_Interface& i2c) {
    cout << "\n=== Synchronous Read Example ===" << endl;

    // Create and send transaction
    I2C_Transaction_Message transaction = createTempReadTransaction();
    transaction.setType(REQUEST_RESPONSE); // Wait for response

    i2c.enque(&transaction);

    // Wait for processing (in real application, use proper synchronization)
    usleep(100000); // 100ms delay

    // Check result
    I2CError error = transaction.getError();
    if (error.hasError) {
        cout << "Read failed with error: " << error.errorCode << endl;
        return;
    }

    // Get temperature data
    vector<I2C_Message>& messages = transaction.getAllMessages();
    if (!messages.empty() && messages[0].m_data.size() >= 2) {
        int16_t rawTemp = (messages[0].m_data[0] << 8) | messages[0].m_data[1];
        rawTemp >>= 4;

        if (rawTemp & 0x800) {
            rawTemp |= 0xF000;
        }

        float temperature = rawTemp * 0.0625;
        cout << "Temperature (sync): " << temperature << "°C" << endl;
    }
}

// Function to register asynchronous periodic read
void asynchronousReadExample(I2C_Interface& i2c) {
    cout << "\n=== Asynchronous Read Example ===" << endl;

    // Create transaction
    I2C_Transaction_Message transaction = createTempReadTransaction();
    transaction.m_transactionDelayUsec = 1000000; // Read every 1 second

    // Register callback
    transaction.registerEventHandle((void*)i2cTransactionCallback, nullptr);

    // Register as periodic event
    uint64_t handle = i2c.registerEvent(transaction);

    cout << "Registered periodic temperature read (handle: " << handle << ")" << endl;
    cout << "Reading temperature every 1 second..." << endl;
    cout << "Press Ctrl+C to stop." << endl;

    // Let it run for some time
    for (int i = 0; i < 10 && g_keepRunning; i++) {
        sleep(1);
    }

    // Unregister event
    i2c.unregisterEvent(handle);
    cout << "Unregistered periodic read." << endl;
}

// Main function
int main() {
    cout << "==================================================" << endl;
    cout << "ApraLinuxUtils I2C Communication Example" << endl;
    cout << "Testing with TMP102 Temperature Sensor" << endl;
    cout << "==================================================" << endl;

    // Setup signal handler
    signal(SIGINT, signalHandler);

    // Create I2C interface
    // Parameters: i2c_path, process_name, frequency_hz, should_print_debug
    I2C_Interface i2c(I2C_BUS_PATH, "I2C_TMP102", 100, true);

    // Check if I2C bus was opened successfully
    if (!i2c.isSuccessfullSetup()) {
        cerr << "Failed to open I2C bus: " << I2C_BUS_PATH << endl;
        cerr << "Please check:" << endl;
        cerr << "  1. I2C device exists (ls -l /dev/i2c-*)" << endl;
        cerr << "  2. User has permission (add user to i2c group)" << endl;
        cerr << "  3. I2C is enabled in system configuration" << endl;
        return 1;
    }

    cout << "I2C interface initialized successfully." << endl;

    // Start the I2C processing thread
    if (i2c.begin() != 0) {
        cerr << "Failed to start I2C thread." << endl;
        return 1;
    }

    cout << "I2C thread started." << endl;

    // Give thread time to initialize
    usleep(100000);

    // Example 1: Write configuration to sensor
    cout << "\n=== Writing Configuration ===" << endl;
    I2C_Transaction_Message configTx = createConfigWriteTransaction();
    configTx.setType(REQUEST_RESPONSE);
    i2c.enque(&configTx);
    usleep(100000);

    if (configTx.getError().hasError) {
        cout << "Warning: Configuration write failed" << endl;
    } else {
        cout << "Configuration written successfully." << endl;
    }

    // Example 2: Synchronous read
    synchronousReadExample(i2c);

    // Example 3: Asynchronous periodic read
    asynchronousReadExample(i2c);

    // Cleanup
    cout << "\n=== Cleaning Up ===" << endl;
    i2c.end();

    cout << "I2C example completed successfully." << endl;

    return 0;
}
