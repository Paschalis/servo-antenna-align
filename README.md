
# Servo-Based Antenna Alignment for Video Transmission

This project optimizes video transmission quality by dynamically aligning a directional antenna using a servo motor controlled by a microcomputer, such as the Raspberry Pi Zero 2 W, Jetson Nano, or Radxa Zero 3W. The system measures WiFi signal strength (RSSI) to adjust the antenna alignment.

## Features
- **Servo Control**: Dynamically rotates the antenna for alignment.
- **WiFi Signal Strength Optimization**: Adjusts alignment for optimal video transmission.
- **Support for Directional Antennas**: Designed for high-quality, long-range streaming.

## Supported Versions
This project offers multiple versions with different features:
1. **[Version 1: RSSI Only](Version%201%3A%20RSSI%20Only/README.md)**: Uses WiFi signal strength (RSSI) to align the antenna dynamically.
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

## **Related Projects**  

### **PCB Yagi-Uda Antenna for UAV Video Transmission**  
This repository contains the design, simulation, and implementation of a **PCB Yagi-Uda antenna** optimized for UAV video transmission. The antenna operates at **2.45 GHz**, providing **high gain** while maintaining a **compact and lightweight** form factor.  

<div align="center">

[![PCB-Yagi-Antenna](https://img.shields.io/badge/🔗%20PCB--Yagi--Antenna-Explore%20Now!-orange?style=for-the-badge)](https://github.com/Paschalis/PCB-Yagi-Antenna)  
[![Stars](https://img.shields.io/github/stars/Paschalis/PCB-Yagi-Antenna?label=🌟%20Stars&style=for-the-badge&color=yellow)](https://github.com/Paschalis/PCB-Yagi-Antenna/stargazers)  [![Forks](https://img.shields.io/github/forks/Paschalis/PCB-Yagi-Antenna?label=🍴%20Forks&style=for-the-badge&color=blue)](https://github.com/Paschalis/PCB-Yagi-Antenna/network/members)   [![Activity](https://img.shields.io/github/last-commit/Paschalis/PCB-Yagi-Antenna?label=⏳%20Last%20Update&style=for-the-badge&color=green)](https://github.com/Paschalis/PCB-Yagi-Antenna/commits/main)  

</div>  

Explore more about the **PCB Yagi-Uda Antenna for UAV Video Transmission** project, including its **design methodology, simulations, and real-world implementation**, by clicking the badge above to access the full documentation and setup guide.  

## License

This repository is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Thank you for visiting servo-antenna-align!**