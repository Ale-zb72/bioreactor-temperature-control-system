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
