# ApraUtils Examples

This directory contains comprehensive examples demonstrating how to use the ApraUtils library for embedded Linux development.

## Available Examples

### 1. GPIO Control (`gpio_example.cpp`)

Demonstrates digital I/O operations:
- Basic output (blinking LED)
- Basic input (reading button state)
- Edge interrupt detection
- Combined input/output (button-controlled LED)

**Hardware**: LED on GPIO 23, Button on GPIO 24, Sensor on GPIO 25

```bash
# Compile
g++ -std=c++14 gpio_example.cpp -I../includes -L../build -lApraUtils -lpthread -o gpio_example

# Run all examples
sudo ./gpio_example

# Run specific example
sudo ./gpio_example output
sudo ./gpio_example input
sudo ./gpio_example interrupt
sudo ./gpio_example combined
```

### 2. I2C Communication (`i2c_example.cpp`)

Demonstrates I2C sensor interfacing:
- Reading temperature from TMP102 sensor
- Writing configuration to I2C devices
- Asynchronous I2C transactions
- Error handling and callbacks

**Hardware**: TMP102 temperature sensor on I2C bus 1, address 0x48

```bash
# Compile
g++ -std=c++14 i2c_example.cpp -I../includes -L../build -lApraUtils -lpthread -o i2c_example

# Run
sudo ./i2c_example
```

### 3. PWM Control (`pwm_example.cpp`)

Demonstrates pulse width modulation:
- LED brightness control with fade effects
- Servo motor positioning (0-180 degrees)
- Different frequencies and duty cycles
- Smooth transitions

**Hardware**: LED on PWM chip 0 channel 0, Servo on PWM chip 0 channel 1

```bash
# Compile
g++ -std=c++14 pwm_example.cpp -I../includes -L../build -lApraUtils -o pwm_example

# Run all examples
sudo ./pwm_example

# Run specific example
sudo ./pwm_example led
sudo ./pwm_example servo
```

### 4. USB Storage Management (`usb_storage_example.cpp`)

Demonstrates USB device detection:
- Detecting USB flash drives
- Monitoring insertion/removal events
- Reading storage information
- Mount/unmount operations

**Hardware**: USB flash drive

```bash
# Compile
g++ -std=c++14 usb_storage_example.cpp -I../includes -L../build -lApraUtils -ludev -o usb_storage_example

# Run basic detection
sudo ./usb_storage_example

# Continuous monitoring
sudo ./usb_storage_example monitor

# Detection only (no mount)
sudo ./usb_storage_example detect-only
```

### 5. Multi-Threading (`threading_example.cpp`)

Demonstrates thread management and message passing:
- Creating custom threads (extending ProcessThread)
- Message passing (REQUEST_ONLY and REQUEST_RESPONSE)
- Producer-consumer pattern
- Request-response pattern
- Thread lifecycle management

**Hardware**: None required

```bash
# Compile
g++ -std=c++14 threading_example.cpp -I../includes -L../build -lApraUtils -lpthread -o threading_example

# Run all examples
./threading_example

# Run specific example
./threading_example basic
./threading_example producer-consumer
./threading_example request-response
./threading_example lifecycle
```

## General Information

### Permissions

Most examples require elevated privileges to access hardware:

```bash
# Run with sudo
sudo ./example_name

# Or add user to appropriate groups
sudo usermod -a -G gpio,i2c,dialout $USER
# Logout and login for changes to take effect
```

### Hardware Safety

**Important**: Be careful when connecting hardware to avoid damage:
- Use appropriate current-limiting resistors for LEDs (typically 220Ω - 1kΩ)
- Verify voltage levels match your board (3.3V or 5V)
- Check pin assignments for your specific hardware platform
- Never connect outputs directly together
- Use pull-up/pull-down resistors for inputs where appropriate

### Compilation Notes

All examples can be compiled with:
```bash
g++ -std=c++14 example_name.cpp -I../includes -L../build -lApraUtils [additional libs] -o example_name
```

Common additional libraries:
- `-lpthread` - Required for threading examples and some hardware interfaces
- `-ludev` - Required for USB storage examples

### Platform-Specific Information

These examples are designed for embedded Linux platforms such as:
- Raspberry Pi
- BeagleBone Black
- NVIDIA Jetson
- Other embedded Linux boards with GPIO, I2C, PWM support

GPIO/PWM pin numbers and I2C bus numbers may vary by platform. Check your board's documentation.

## Example Structure

Each example follows this structure:

1. **Copyright Header**: MIT License with Apra Labs copyright
2. **Documentation**: Purpose, hardware setup, compilation, execution
3. **Includes**: ApraUtils and standard library headers
4. **Helper Functions**: Utility functions for the examples
5. **Example Functions**: Individual demonstrations
6. **Main Function**: Command-line argument parsing and execution
7. **Error Handling**: Proper error checking and reporting
8. **Cleanup**: Resource deallocation and GPIO/hardware cleanup

## Learning Path

Recommended order for learning:

1. **GPIO Example** - Start here to understand basic digital I/O
2. **PWM Example** - Learn analog-like control with digital signals
3. **I2C Example** - Understand sensor communication protocols
4. **Threading Example** - Master concurrent programming patterns
5. **USB Storage Example** - Learn system integration and device management

## Troubleshooting

### Permission Denied

```bash
# Solution 1: Run with sudo
sudo ./example_name

# Solution 2: Add user to groups
sudo usermod -a -G gpio,i2c,dialout $USER
```

### GPIO/I2C Device Not Found

```bash
# Check if devices exist
ls /sys/class/gpio/
ls /dev/i2c-*

# Enable I2C if needed (Raspberry Pi)
sudo raspi-config
# Navigate to: Interface Options -> I2C -> Enable
```

### PWM Not Available

```bash
# Check PWM availability
ls /sys/class/pwm/

# Some platforms require device tree overlays
# Raspberry Pi example:
# Add to /boot/config.txt:
# dtoverlay=pwm-2chan
```

### Compilation Errors

```bash
# Ensure ApraUtils is built
cd ../build
cmake ..
make

# Check library path
ls ../build/libApraUtils.a

# Verify include path
ls ../includes/ApraUtils.h
```

## Contributing Examples

Have a useful example? Consider contributing:
1. Follow the existing example structure
2. Include comprehensive comments
3. Add hardware requirements
4. Test on real hardware
5. Submit a pull request

See [CONTRIBUTING.md](../CONTRIBUTING.md) for details.

## Support

- **Issues**: [GitHub Issues](https://github.com/Apra-Labs/ApraUtils/issues)
- **Discussions**: [GitHub Discussions](https://github.com/Apra-Labs/ApraUtils/discussions)
- **Documentation**: [Main README](../README.md)

## License

All examples are licensed under the MIT License - see [LICENSE](../LICENSE) for details.

---

**Happy Coding!** 🚀
