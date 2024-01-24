# ShadowBot Navigation System

<img src="https://github.com/James-Bray19/Shadow-Bot/assets/47334864/9623b240-3c67-4408-a0d1-930be017ca8c" style="display: block; margin: auto; width: 75%;" />

## Overview
The Shadowbot Navigation code is designed for a two-wheel ShadowBot equipped with two proximity sensors, four LEDs, two motors, and two infrared sensors to follow an IR beacon. This README provides an overview of the system, its components, and how to use the provided code.

## Components
1. **Proximity Sensors:** The robot is equipped with two proximity sensors, located on the left and right sides, to detect obstacles and adjust its movement accordingly.

2. **LEDs:** Four LEDs are used to indicate different states of the robot, such as obstacle detection or successful beacon tracking.

3. **Motors:** The robot features two motors, each controlled by an H-bridge, enabling it to move forward, backward, turn, and stop.

4. **Infrared Sensors:** The system includes two infrared sensors that help the robot follow an IR beacon.

5. **IR Beacon:** An external IR beacon is used as a reference point for the robot's navigation.

## Code Overview
The provided code, located in the `general.c` file, is structured as follows:

### Initialization
- Configuration settings for oscillator, watchdog timer, low voltage programming, and power-up timer.
- Definition of clock frequency and pin assignments for H-bridge, LEDs, beacon sensors, and encoders.

### Variables
- `setpoint_distance`: Minimum distance for obstacle detection.
- `ang2ticks`: Ratio of encoder ticks to degrees.
- `motor_speed`: Motor speed (0-255).

### Functions
1. **`readleftADC()` and `readrightADC()`**
   - Read values from left and right proximity sensors using ADC.

2. **`wait10ms(int del)`**
   - Delay function for `del*10` milliseconds.

3. **`turn(int dir, int ang)`**
   - Rotate the robot based on direction (`dir`) and angle (`ang`).

4. **`acc(int dir, int dur)`**
   - Accelerate the robot based on direction (`dir`) and duration (`dur`).

5. **`flashLEDs(int dur)`**
   - Flash all LEDs three times over a specified duration.

6. **`facebeacon()`**
   - Rotate the robot to face the IR beacon.

### Main Loop
- Continuous loop where the robot faces the beacon, detects obstacles, and adjusts its movement accordingly.

## Usage
1. **Hardware Setup:**
   - Connect the proximity sensors, LEDs, motors, and infrared sensors as per the defined pin assignments.
   - Place the external IR beacon as the reference point.

2. **Code Upload:**
   - Upload the provided `general.c` code to the microcontroller of the ShadowBot.

3. **Execution:**
   - Power on the ShadowBot and observe its behavior.
   - The robot will continuously face the IR beacon, detect obstacles, and navigate around them.

## Customization
- Adjust the `setpoint_distance`, `ang2ticks`, and `motor_speed` variables to fine-tune the robot's behavior.
- Modify the code within the main loop to add additional behaviors or features.

## Dependencies
- The code relies on the availability and proper functioning of the specified hardware components.

## Contributors
- James Bray
- Sam Davis
- Fabian Chrzanowski
