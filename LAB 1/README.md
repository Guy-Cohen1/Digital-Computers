# Lab 1 - System Programming, GPIO, Interrupts, LPM

## Overview
This folder contains the files and documentation for **Lab 1** in the System Programming course. The lab focuses on:
- **GPIO Configuration** and manipulation
- **Interrupt Handling**
- **Low-Power Modes (LPM)**
- Implementation of a software architecture using **Finite State Machines (FSM)** with **Interrupt-Driven Design**.

---

## Practical Requirements
1. **FSM Design**:
   - Software should follow an FSM architecture triggered by external interrupts.  
   - CPU enters sleep mode (LPM) when idle and transitions between states using interrupts.  
   - Ensure code is modular with clear separation into layers:
     - **BSP**: Peripheral configurations.
     - **HAL**: Low-level driver routines.
     - **API**: High-level abstractions for portability.
     - **Application**: Main FSM implementation and user interface.  

2. **State Descriptions**:
   - **State 0 (Idle)**: Enter sleep mode with LEDs off.  
   - **State 1**: Perform binary counting on 8 LEDs (up/down alternately) with a 0.5-second delay for 10 seconds.  
   - **State 2**: Single LED shifts right-to-left with a 0.5-second delay for 7 seconds.  
   - **State 3**: Generate a 4 kHz PWM signal with a 75% duty cycle on `P2.7`.

  ---


## Conclusion
Lab 1 provides an in-depth exploration of System Programming concepts, focusing on hardware-software interaction through GPIO, interrupts, and low-power modes.
