### Final Project - Design and Implementation of a Control System for a Stepper Motor-Based Machine with Manual and Remote Control 

---

#### **Table of Contents**
1. **Project Objective**  
2. **Project Task Requirements**  
    - **Manual Control of the Motor-Based Machine**  
    - **PC Painter Using Joystick**  
    - **Stepper Motor Calibration**  
    - **Script Mode**  
3. **Technical Explanations – Sensors and Servo Motor**  
    - **Analog Joystick**  
    - **Stepper Motor**  
    - **PC User Interface**  
4. **Preliminary Report (10%)**  
5. **Project Grading Structure**  

---

#### **Project Objective**
1. **System Design**: Develop an embedded system using an MCU to control a stepper motor-based machine via:  
    - Manual control using an analog joystick.  
    - Remote control through a personal computer (PC) via a serial communication channel.  
2. **Key Features**: Emphasis on high performance, low power consumption, and strict **hard real-time** operation with high precision.  
3. **Embedded Software Development**: Implemented in C/C++ with a focus on:  
    - Embedded systems using an FSM-based kernel.  
    - Layered abstraction methodology for a sensor-rich, robust system.  
4. **User Interface**: The PC serves as a GUI interface for displaying operations and sending user commands, implemented using Python (e.g., Tkinter or PySimpleGUI). Communication with the MCU occurs over RS-232 serial communication.  
5. **Script Automation**: The interface supports the creation, transmission, and execution of high-level scripts stored in the MCU’s FLASH memory.  

---

#### **Project Task Requirements**
- **Software Architecture**:  
   - FSM-based operation kernel, triggered by RX interrupts from PC via UART.  
   - Layered design for easy portability between MSP430 MCU families.  
- **Pre-Implementation Requirements**:  
   - Draw detailed FSM diagrams for both the MCU and PC communication support.  
   - Avoid delays except for debounce in interrupt routines.  
   - The system is controlled exclusively via joystick or PC (no additional buttons or switches are allowed).  

---

#### **System Features**
1. **Manual Control**:  
   - Dynamic control of the stepper motor’s pointer angle (360° range) using an analog joystick.  
   - Feedback system for zero calibration before operation.  
2. **PC Painter**:  
   - Utilize the joystick as a “pencil” for drawing on the PC screen.  
   - Three modes: Draw, Erase, and Neutral (movement without drawing).  
3. **Stepper Motor Calibration**:  
   - Single-time calibration to measure step size and the number of steps per full rotation.  
   - Save calibration data in the MCU’s FLASH memory.  
4. **Script Mode**:  
   - Execute pre-defined scripts to automate system functionality.  
   - Support up to three independent scripts stored in FLASH memory, with commands executed in **hard real-time**.  

---

#### **Technical Overview**
- **Joystick**:  
   - Outputs proportional voltage changes based on X/Y axis tilt and provides a press action as a button.  
- **Stepper Motor**:  
   - Operates with precision step control (e.g., 0.088° per step), requiring calibration to account for physical variability.  
- **Hardware Connections**:  
   - MCU pins for UART, motor phases, joystick axes, and button press.  
   - LCD interfacing for display output.  

---

This project serves as a culmination of the theoretical and practical knowledge gained in the Digital Computer Architecture course, emphasizing embedded systems, precise control algorithms, and robust design practices.
