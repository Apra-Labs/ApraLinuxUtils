/*
 * MockHardware.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef TESTS_MOCKS_MOCKHARDWARE_H_
#define TESTS_MOCKS_MOCKHARDWARE_H_

#include <map>
#include <vector>
#include <string>
#include <cstdint>

namespace apra
{
namespace testing
{

/**
 * MockGPIO - Simple state-based mock for GPIO hardware
 * Simulates GPIO pins without requiring actual hardware
 */
class MockGPIO
{
public:
    MockGPIO() : m_isExported(false), m_isOpen(false), m_isRead(false),
                 m_value(0), m_edge(0) {}

    void setExported(bool exported) { m_isExported = exported; }
    bool isExported() const { return m_isExported; }

    void setOpen(bool open) { m_isOpen = open; }
    bool isOpen() const { return m_isOpen; }

    void setDirection(bool isRead) { m_isRead = isRead; }
    bool isReadDirection() const { return m_isRead; }

    void setValue(int value) { m_value = value; }
    int getValue() const { return m_value; }

    void setEdge(int edge) { m_edge = edge; }
    int getEdge() const { return m_edge; }

private:
    bool m_isExported;
    bool m_isOpen;
    bool m_isRead;
    int m_value;
    int m_edge;
};

/**
 * MockI2C - Simple state-based mock for I2C bus
 * Simulates I2C device communication without actual hardware
 */
class MockI2C
{
public:
    MockI2C() : m_isOpen(false), m_deviceAddress(0), m_lastWriteSize(0),
                m_lastReadSize(0) {}

    void setOpen(bool open) { m_isOpen = open; }
    bool isOpen() const { return m_isOpen; }

    void setDeviceAddress(uint8_t address) { m_deviceAddress = address; }
    uint8_t getDeviceAddress() const { return m_deviceAddress; }

    // Write operations
    void writeData(const std::vector<uint8_t>& data) {
        m_writeBuffer = data;
        m_lastWriteSize = data.size();
    }

    const std::vector<uint8_t>& getWriteBuffer() const { return m_writeBuffer; }
    size_t getLastWriteSize() const { return m_lastWriteSize; }

    // Read operations
    void setReadData(const std::vector<uint8_t>& data) {
        m_readBuffer = data;
    }

    std::vector<uint8_t> readData(size_t size) {
        m_lastReadSize = size;
        if (size > m_readBuffer.size()) {
            size = m_readBuffer.size();
        }
        return std::vector<uint8_t>(m_readBuffer.begin(), m_readBuffer.begin() + size);
    }

    size_t getLastReadSize() const { return m_lastReadSize; }

    // Register simulation
    void setRegister(uint8_t reg, uint8_t value) {
        m_registers[reg] = value;
    }

    uint8_t getRegister(uint8_t reg) const {
        auto it = m_registers.find(reg);
        if (it != m_registers.end()) {
            return it->second;
        }
        return 0;
    }

    void clearRegisters() { m_registers.clear(); }

private:
    bool m_isOpen;
    uint8_t m_deviceAddress;
    std::vector<uint8_t> m_writeBuffer;
    std::vector<uint8_t> m_readBuffer;
    size_t m_lastWriteSize;
    size_t m_lastReadSize;
    std::map<uint8_t, uint8_t> m_registers;
};

/**
 * MockPWM - Simple state-based mock for PWM hardware
 * Simulates PWM signal generation without actual hardware
 */
class MockPWM
{
public:
    MockPWM() : m_isExported(false), m_isEnabled(false), m_period(0),
                m_dutyCycle(0), m_frequency(0.0) {}

    void setExported(bool exported) { m_isExported = exported; }
    bool isExported() const { return m_isExported; }

    void setEnabled(bool enabled) { m_isEnabled = enabled; }
    bool isEnabled() const { return m_isEnabled; }

    void setPeriod(uint64_t period) { m_period = period; }
    uint64_t getPeriod() const { return m_period; }

    void setDutyCycle(uint64_t dutyCycle) { m_dutyCycle = dutyCycle; }
    uint64_t getDutyCycle() const { return m_dutyCycle; }

    void setFrequency(double frequency) { m_frequency = frequency; }
    double getFrequency() const { return m_frequency; }

    // Calculated duty cycle percentage
    double getDutyCyclePercent() const {
        if (m_period == 0) return 0.0;
        return (static_cast<double>(m_dutyCycle) / static_cast<double>(m_period)) * 100.0;
    }

private:
    bool m_isExported;
    bool m_isEnabled;
    uint64_t m_period;
    uint64_t m_dutyCycle;
    double m_frequency;
};

} // namespace testing
} // namespace apra

#endif /* TESTS_MOCKS_MOCKHARDWARE_H_ */
