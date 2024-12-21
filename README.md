
# Servo-Based Antenna Alignment for Video Transmission

This project optimizes video transmission quality by dynamically aligning a directional antenna using a servo motor controlled by a microcomputer, such as the Raspberry Pi Zero 2 W, Jetson Nano, or Radxa Zero 3W. The system measures WiFi signal strength (RSSI) to adjust the antenna alignment.

## Features
- **Servo Control**: Dynamically rotates the antenna for alignment.
- **WiFi Signal Strength Optimization**: Adjusts alignment for optimal video transmission.
- **Support for Directional Antennas**: Designed for high-quality, long-range streaming.

## Supported Versions
This project offers multiple versions with different features:
1. **[Version 1: RSSI Only](Version%201:%20RSSI%20Only/README.md)**: Uses WiFi signal strength (RSSI) to align the antenna dynamically.
2. **Version 2: GPS + RSSI**: Combines GPS and WiFi signal strength for more accurate alignment.
3. **Version 3: Swarming with Mesh Network, GPS, and RSSI**: Adds mesh network capabilities for UAV swarming and advanced alignment.

## Supported Devices
1. **Raspberry Pi Zero 2 W** (uses WiringPi for GPIO control).
2. **Radxa Zero 3W or Jetson Nano** (uses libgpiod for GPIO control).

## Requirements

### **Hardware**
1. Raspberry Pi Zero 2 W, Jetson Nano, or Radxa Zero 3W.
2. Servo motor (PWM-compatible).
3. Directional antenna for WiFi (e.g., Yagi or panel antenna).

### **Software**
1. **WiringPi Library** (for Raspberry Pi Zero 2 W).
2. **libgpiod** (for Jetson Nano and Radxa Zero 3W).
3. **GStreamer**: For video streaming over WiFi.
## License

This repository is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Thank you for visiting servo-antenna-align!**