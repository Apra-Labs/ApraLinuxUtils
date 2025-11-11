/*
 * threading_example.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

/*
 * Multi-Threading with ProcessThread Example
 *
 * This example demonstrates how to use the ApraLinuxUtils ProcessThread class to:
 * 1. Create custom threads by extending ProcessThread
 * 2. Implement message passing between threads
 * 3. Use REQUEST_ONLY and REQUEST_RESPONSE message types
 * 4. Demonstrate proper thread lifecycle (start, stop, cleanup)
 * 5. Show multiple threads communicating with each other
 * 6. Handle thread synchronization and message queuing
 *
 * Concepts Demonstrated:
 * ----------------------
 * - Producer-Consumer pattern
 * - Request-Response pattern
 * - Message-based thread communication
 * - Thread lifecycle management
 * - Custom message types
 *
 * Hardware Requirements:
 * ---------------------
 * - Any Linux-based system with pthread support
 * - No special hardware needed
 *
 * Compilation:
 * ------------
 * g++ -std=c++14 threading_example.cpp -o threading_example -lApraLinuxUtils -lpthread
 *
 * Execution:
 * ----------
 * ./threading_example
 *
 * Notes:
 * ------
 * - This example is purely software-based
 * - Demonstrates thread-safe message passing
 * - Shows how to create custom thread classes
 */

#include <ApraUtils.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cmath>

using namespace apra;
using namespace std;

// Control flag
volatile bool g_keepRunning = true;

// Signal handler for graceful shutdown
void signalHandler(int signum) {
    cout << "\nInterrupt signal (" << signum << ") received. Shutting down..." << endl;
    g_keepRunning = false;
}

// Custom message types for our examples
class DataMessage : public Message {
public:
    int32_t data;
    string description;

    DataMessage(int32_t value, const string& desc) : data(value), description(desc) {
        setType(REQUEST_ONLY);
    }
};

class ComputeMessage : public Message {
public:
    double inputValue;
    double result;
    string operation;
    bool isProcessed;

    ComputeMessage(double value, const string& op)
        : inputValue(value), result(0.0), operation(op), isProcessed(false) {
        setType(REQUEST_RESPONSE);
    }
};

// Example 1: Simple Data Logger Thread
// Receives messages and logs them
class DataLoggerThread : public ProcessThread {
public:
    DataLoggerThread() : ProcessThread("DataLogger", 0) {
        messageCount = 0;
    }

    virtual ~DataLoggerThread() {}

    void process(Message* msg) override {
        if (msg == nullptr) {
            return;
        }

        // Cast to our custom message type
        DataMessage* dataMsg = dynamic_cast<DataMessage*>(msg);
        if (dataMsg != nullptr) {
            messageCount++;
            cout << "[DataLogger] Message #" << messageCount
                 << ": " << dataMsg->description
                 << " = " << dataMsg->data << endl;

            // Simulate some processing time
            usleep(50000); // 50ms
        }

        // Clean up message
        delete msg;
    }

    int getMessageCount() const {
        return messageCount;
    }

private:
    int messageCount;
};

// Example 2: Compute Thread
// Performs calculations and responds with results
class ComputeThread : public ProcessThread {
public:
    ComputeThread() : ProcessThread("ComputeEngine", 100) { // 100Hz processing rate
        computeCount = 0;
    }

    virtual ~ComputeThread() {}

    void process(Message* msg) override {
        if (msg == nullptr) {
            return;
        }

        ComputeMessage* computeMsg = dynamic_cast<ComputeMessage*>(msg);
        if (computeMsg != nullptr) {
            computeCount++;

            cout << "[ComputeEngine] Processing request #" << computeCount
                 << ": " << computeMsg->operation
                 << "(" << computeMsg->inputValue << ")" << endl;

            // Perform computation based on operation
            if (computeMsg->operation == "square") {
                computeMsg->result = computeMsg->inputValue * computeMsg->inputValue;
            }
            else if (computeMsg->operation == "sqrt") {
                computeMsg->result = sqrt(computeMsg->inputValue);
            }
            else if (computeMsg->operation == "double") {
                computeMsg->result = computeMsg->inputValue * 2.0;
            }
            else {
                computeMsg->result = computeMsg->inputValue;
            }

            computeMsg->isProcessed = true;

            cout << "[ComputeEngine] Result: " << computeMsg->result << endl;

            // For REQUEST_RESPONSE, the message will be returned to response queue
            // Note: ProcessThread handles this automatically
        }

        // Do not delete REQUEST_RESPONSE messages - caller needs them
    }

    int getComputeCount() const {
        return computeCount;
    }

private:
    int computeCount;
};

// Example 3: Producer Thread
// Generates data periodically
class ProducerThread : public ProcessThread {
public:
    ProducerThread(DataLoggerThread* logger)
        : ProcessThread("Producer", 2), // 2Hz = produce every 500ms
          targetLogger(logger), producedCount(0) {
    }

    virtual ~ProducerThread() {}

    void process(Message* msg) override {
        // This thread doesn't receive messages, it only produces them
        // The ProcessThread framework will call this periodically based on frequency

        if (targetLogger == nullptr || shouldIquit()) {
            return;
        }

        // Generate some data
        int32_t value = rand() % 1000;
        producedCount++;

        string description = "Produced data item #" + to_string(producedCount);
        DataMessage* dataMsg = new DataMessage(value, description);

        cout << "[Producer] Sending: " << description << " = " << value << endl;

        // Send to logger
        targetLogger->enque(dataMsg);
    }

    int getProducedCount() const {
        return producedCount;
    }

private:
    DataLoggerThread* targetLogger;
    int producedCount;
};

// Example 1: Simple message passing with REQUEST_ONLY
void simpleMessagePassingExample() {
    cout << "\n==================================================" << endl;
    cout << "Example 1: Simple Message Passing (REQUEST_ONLY)" << endl;
    cout << "==================================================" << endl;
    cout << "Demonstrates one-way message communication." << endl;
    cout << "Main thread sends messages to logger thread." << endl;
    cout << "==================================================" << endl;

    // Create logger thread
    DataLoggerThread logger;

    // Start thread
    if (logger.begin() != 0) {
        cerr << "Failed to start logger thread." << endl;
        return;
    }

    cout << "\nLogger thread started. Sending 10 messages..." << endl;
    sleep(1);

    // Send messages
    for (int i = 1; i <= 10 && g_keepRunning; i++) {
        int32_t value = i * 10;
        string desc = "Test message " + to_string(i);

        DataMessage* msg = new DataMessage(value, desc);
        logger.enque(msg);

        cout << "[Main] Sent message: " << desc << " = " << value << endl;

        usleep(200000); // 200ms between messages
    }

    // Wait for processing to complete
    cout << "\nWaiting for messages to be processed..." << endl;
    sleep(2);

    // Stop thread
    cout << "\nStopping logger thread..." << endl;
    logger.end();

    cout << "Total messages processed: " << logger.getMessageCount() << endl;
    cout << "Example 1 completed." << endl;
}

// Example 2: Request-Response pattern
void requestResponseExample() {
    cout << "\n==================================================" << endl;
    cout << "Example 2: Request-Response Pattern" << endl;
    cout << "==================================================" << endl;
    cout << "Demonstrates REQUEST_RESPONSE message type." << endl;
    cout << "Main thread sends compute requests and waits for results." << endl;
    cout << "==================================================" << endl;

    // Create compute thread
    ComputeThread computer;

    // Start thread
    if (computer.begin() != 0) {
        cerr << "Failed to start compute thread." << endl;
        return;
    }

    cout << "\nCompute thread started." << endl;
    sleep(1);

    // Test different operations
    struct TestCase {
        double value;
        string operation;
    };

    TestCase tests[] = {
        {16.0, "square"},
        {25.0, "sqrt"},
        {42.0, "double"},
        {100.0, "square"},
        {144.0, "sqrt"}
    };

    cout << "\nSending compute requests..." << endl;

    for (const auto& test : tests) {
        if (!g_keepRunning) break;

        cout << "\n[Main] Requesting: " << test.operation
             << "(" << test.value << ")" << endl;

        // Create REQUEST_RESPONSE message
        ComputeMessage* msg = new ComputeMessage(test.value, test.operation);

        // Send to compute thread
        computer.enque(msg);

        // Wait for response (in real application, use proper synchronization)
        // For demonstration, we poll and check
        int maxWaitMs = 1000;
        int waitedMs = 0;

        while (!msg->isProcessed && waitedMs < maxWaitMs) {
            usleep(10000); // 10ms
            waitedMs += 10;

            // Try to get response from response queue
            Message* response = computer.dequeue();
            if (response != nullptr && response == msg) {
                break;
            }
        }

        if (msg->isProcessed) {
            cout << "[Main] Received result: " << msg->result << endl;
        } else {
            cout << "[Main] Warning: Request timed out" << endl;
        }

        delete msg;
        usleep(500000); // 500ms between requests
    }

    // Stop thread
    cout << "\n\nStopping compute thread..." << endl;
    computer.end();

    cout << "Total computations: " << computer.getComputeCount() << endl;
    cout << "Example 2 completed." << endl;
}

// Example 3: Multiple communicating threads
void multipleThreadsExample() {
    cout << "\n==================================================" << endl;
    cout << "Example 3: Multiple Communicating Threads" << endl;
    cout << "==================================================" << endl;
    cout << "Demonstrates producer-consumer pattern." << endl;
    cout << "Producer thread generates data, Logger thread processes it." << endl;
    cout << "==================================================" << endl;

    // Create threads
    DataLoggerThread logger;
    ProducerThread producer(&logger);

    // Start threads
    cout << "\nStarting threads..." << endl;

    if (logger.begin() != 0) {
        cerr << "Failed to start logger thread." << endl;
        return;
    }

    if (producer.begin() != 0) {
        cerr << "Failed to start producer thread." << endl;
        logger.end();
        return;
    }

    cout << "Both threads running." << endl;
    cout << "Producer will generate data every 500ms." << endl;
    cout << "Logger will process and log the data." << endl;
    cout << "\nRunning for 10 seconds..." << endl;

    // Let them run
    for (int i = 10; i > 0 && g_keepRunning; i--) {
        cout << "\n[Main] " << i << " seconds remaining..." << endl;
        sleep(1);
    }

    // Stop threads (stop producer first)
    cout << "\n\nStopping threads..." << endl;
    cout << "Stopping producer..." << endl;
    producer.end();

    // Give logger time to process remaining messages
    sleep(1);

    cout << "Stopping logger..." << endl;
    logger.end();

    // Show statistics
    cout << "\n--- Statistics ---" << endl;
    cout << "Producer generated: " << producer.getProducedCount() << " items" << endl;
    cout << "Logger processed:   " << logger.getMessageCount() << " items" << endl;
    cout << "Example 3 completed." << endl;
}

// Example 4: Thread lifecycle demonstration
void threadLifecycleExample() {
    cout << "\n==================================================" << endl;
    cout << "Example 4: Thread Lifecycle Management" << endl;
    cout << "==================================================" << endl;
    cout << "Demonstrates proper thread start, stop, and restart." << endl;
    cout << "==================================================" << endl;

    DataLoggerThread logger;

    // First lifecycle: Start -> Use -> Stop
    cout << "\n--- First lifecycle ---" << endl;
    cout << "Starting thread..." << endl;
    logger.begin();
    cout << "Thread name: " << logger.getName() << endl;

    cout << "Sending 3 messages..." << endl;
    for (int i = 1; i <= 3; i++) {
        DataMessage* msg = new DataMessage(i * 100, "Lifecycle test " + to_string(i));
        logger.enque(msg);
        usleep(100000);
    }

    sleep(1);
    cout << "Stopping thread..." << endl;
    logger.end();
    cout << "First lifecycle complete. Messages processed: "
         << logger.getMessageCount() << endl;

    // Small delay
    sleep(1);

    // Second lifecycle: Restart and use again
    cout << "\n--- Second lifecycle ---" << endl;
    cout << "Restarting thread..." << endl;
    logger.begin();

    cout << "Sending 3 more messages..." << endl;
    for (int i = 4; i <= 6; i++) {
        DataMessage* msg = new DataMessage(i * 100, "Lifecycle test " + to_string(i));
        logger.enque(msg);
        usleep(100000);
    }

    sleep(1);
    cout << "Stopping thread..." << endl;
    logger.end();
    cout << "Second lifecycle complete. Total messages processed: "
         << logger.getMessageCount() << endl;

    cout << "\nExample 4 completed." << endl;
}

// Main function
int main() {
    cout << "==================================================" << endl;
    cout << "ApraLinuxUtils ProcessThread Example" << endl;
    cout << "==================================================" << endl;
    cout << "This example demonstrates thread management and" << endl;
    cout << "message-based inter-thread communication." << endl;
    cout << "\nPress Ctrl+C to interrupt at any time." << endl;
    cout << "==================================================" << endl;

    // Setup signal handler
    signal(SIGINT, signalHandler);

    // Seed random number generator for producer example
    srand(time(nullptr));

    // Run examples
    if (g_keepRunning) {
        simpleMessagePassingExample();
    }

    if (g_keepRunning) {
        cout << "\n\nPress Enter to continue to next example...";
        cin.get();
        requestResponseExample();
    }

    if (g_keepRunning) {
        cout << "\n\nPress Enter to continue to next example...";
        cin.get();
        multipleThreadsExample();
    }

    if (g_keepRunning) {
        cout << "\n\nPress Enter to continue to final example...";
        cin.get();
        threadLifecycleExample();
    }

    cout << "\n==================================================" << endl;
    cout << "All threading examples completed successfully!" << endl;
    cout << "==================================================" << endl;
    cout << "\nKey Takeaways:" << endl;
    cout << "  1. ProcessThread provides easy thread management" << endl;
    cout << "  2. Messages enable safe inter-thread communication" << endl;
    cout << "  3. REQUEST_ONLY for one-way messages" << endl;
    cout << "  4. REQUEST_RESPONSE for request-reply pattern" << endl;
    cout << "  5. Proper lifecycle: begin() -> use -> end()" << endl;
    cout << "  6. Thread-safe message queues built-in" << endl;
    cout << "==================================================" << endl;

    return 0;
}
