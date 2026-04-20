# bioreactor-temperature-control-system
Design and implementation of a temperature control system for a small-scale bioreactor, focused on maintaining stable thermal conditions required for biological or chemical processes.

# Overview
This project presents the development of a temperature regulation system capable of maintaining a controlled thermal environment. The system integrates hardware and software components to regulate temperature using feedback control.

The design is suitable for applications where maintaining a stable temperature range is critical.

# Control System
Temperature regulation is achieved using a PI controller implemented on an Arduino.

Control Parameters
Parameter	| Value
Kp	      | 5.5
Ki	      | 0.6
Setpoint	| 38.5 °C
Range	    | 37.5 – 39.5 °C

Control Features
Open-loop mode for system analysis
Closed-loop mode for automatic regulation
Output saturation (0–100%)
Safety constraints to prevent overheating

# System Architecture

The system integrates sensing, control, and actuation:
- Sensor: DS18B20 (temperature measurement)
- Controller: Arduino Uno
- Actuation: Heating resistors
- Power stage: LM317 + TIP147 transistor
- Interface: LCD 16x2 (I2C)
- Synchronization: Zero-cross detection

# System Diagram
bioreactor-temperature-control-system/bioreactor_diagram.jpg

# Experimental Results

Three sets of controller parameters were tested to evaluate system performance.

- Test 1: Kp = 4.5, Ki = 0.4
- Test 2: Kp = 5.0, Ki = 0.5
- Test 3: Kp = 5.5, Ki = 0.6

Key Result

The third configuration provided the best performance:

- Stable temperature regulation
- Error within approximately ±0.5 °C of setpoint
- Smooth system response

# System Response

The system exhibits gradual heating due to thermal inertia and stabilizes around the desired setpoint.

# Metodology

1. Sensor validation and testing
2. Open-loop system characterization
3. Implementation of PI control
4. Parameter tuning through experimental trials
5. Data acquisition using Arduino and serial communication
6. Data processing and visualization in MATLAB

# Hardware Implementation

- Heating resistors attached to a container
- External temperature sensing
- Magnetic stirring (optional for homogenization)
- PCB implementation for circuit stability
- Heat dissipation for power components

# Engineering Cosiderations

- Thermal systems exhibit slow dynamics, limiting the need for derivative control
- Heat dissipation in power components is critical
- Heating and cooling rates are not symmetric
- Controller tuning depends on operating conditions

# Applications

- Bioreactors and incubation systems
- Temperature-sensitive processes
- Biomedical and laboratory equipment
- Control systems education
