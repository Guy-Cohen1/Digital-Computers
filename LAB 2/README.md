# Lab 2 - Timers, ADC, and DAC  

## Overview  
This folder contains the files for **Lab 2**, focusing on:  
- **Timers**: Capture and Compare modes using TimerB for frequency measurement and PWM generation.  
- **ADC**: Configuring the 12-bit ADC for analog input sampling and processing.  
- **DAC**: Using the DAC for generating analog output signals.  

The lab emphasizes the use of Finite State Machines (FSM) and modular software design to create portable and reusable embedded system applications.  

---

## Practical Objectives  

### Hardware Setup:  
1. **Input Buttons**:  
   - `SW0` (P2.0): General-purpose input switch.  
   - `PB0`–`PB2` (P1.0–P1.2): External interrupt-triggering buttons.  
2. **Analog Input**:  
   - `POT` (P1.3): Analog potentiometer input.  
3. **LCD Screen**:  
   - Data lines connected to P1.4–P1.7 in 4-bit mode.  
   - Control lines connected to P2.5–P2.7.  
4. **Frequency Generator**:  
   - Input to `P2.4` for capture mode functionality.  
5. **PWM Output**:  
   - `P2.2`: Generates PWM signals for the buzzer and scope testing.  

---

### Software Architecture:  
The software is designed using a **Simple FSM** with abstraction layers for portability:  
- **BSP Layer**: Hardware-specific configurations.  
- **HAL Layer**: Drivers for peripherals (e.g., TimerB, ADC, DAC, and LCD).  
- **API Layer**: High-level abstractions for user interfaces and data handling.  
- **Application Layer**: FSM logic and interrupt-driven state management.  

---

## Functional Requirements  

### State Descriptions:  
1. **State 0 (Idle)**:  
   - Enter sleep mode (LPM).  
   - Reset or exit upon button press.  

2. **State 1 (Frequency Measurement)**:  
   - Measure external clock signal frequency via `P2.4` (capture input).  
   - Display real-time frequency in Hz on the LCD with efficient screen updates.  

3. **State 2 (Stopwatch)**:  
   - Implement a countdown and count-up timer displayed on the LCD.  
   - Controlled using `SW0` for start/stop and `PB1` for exit.  

4. **State 3 (Tone Generation)**:  
   - Read potentiometer input via ADC (`P1.3`) to dynamically adjust tone frequency (`P2.2`, PWM output).  
   - Frequency range: 1 kHz to 2.5 kHz, using linear transformation of ADC values.  

---

## Key Learnings  

- **Timers**: Mastering capture and compare functionality for input measurement and PWM signal generation.  
- **ADC/DAC**: Implementing efficient analog data acquisition and output.  
- **FSM Design**: Structuring embedded applications for modularity and portability.  
- **Real-Time Debugging**: Using debugging tools for timing analysis and performance optimization.  

---

## Files in this Folder   
   - `main.c`: Main application logic, including FSM implementation.  
   - `bsp.c/h`: Hardware initialization (GPIO, timers, ADC, DAC, LCD).  
   - `hal.c/h`: Low-level drivers for peripheral modules.  
   - `api.c/h`: High-level functions for LCD updates, frequency measurement, etc.  

---

## How to Run  

1. Connect the hardware as described in the **Hardware Setup** section.  
2. Compile and upload the provided code to the microcontroller.  
3. Test each state using the appropriate inputs (`PB0`, `PB1`, `PB2`, and `SW0`).  
4. Observe outputs on the LCD and oscilloscope for validation.  
