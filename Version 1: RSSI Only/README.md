# Antenna Alignment Based on Wi-Fi Signal Strength


This project adjusts an antenna's servo motor to the optimal position based on the Wi-Fi signal strength (RSSI) received by a Raspberry Pi. The servo motor rotates to find the best alignment by measuring Wi-Fi signal strength using `iwconfig`.

## Features

- Measures Wi-Fi signal strength (RSSI) using `iwconfig`.
- Adjusts the antenna's position with a servo motor.
- Optimizes the antenna's alignment for maximum signal strength.

## Operation

- The program rotates the servo from 0° to 180° in 5° increments.
- Measures signal strength at each angle.
- Adjusts to the angle with the best signal.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
