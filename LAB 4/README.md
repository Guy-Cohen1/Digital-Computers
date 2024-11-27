### **Lab 4: Universal Asynchronous Receiver/Transmitter (UART) Module**

#### **Objective**
The goal of Lab 4 is to implement a bidirectional UART-based asynchronous serial communication system between a PC and an MSP430G2553 microcontroller. This involves developing both PC-side and microcontroller-side software that adheres to specific communication protocols and utilizes interrupt-driven mechanisms for efficient data handling.

---

#### **Overview**
- **UART Communication**: The microcontroller communicates with the PC over a USB connection, which internally translates into UART protocol signals. Specifically, pins P1.1 (RX) and P1.2 (TX) of the MSP430 are used for serial communication.
- **Interrupt-Driven Operation**: The microcontroller code must handle UART communication using interrupt routines for both transmission and reception, ensuring asynchronous operation without busy waiting.
- **PC Application**: A PC-side application, written in a high-level programming language (e.g., Python, C++, MATLAB), communicates with the microcontroller by sending commands and receiving responses. The application uses a finite state machine (FSM) for processing commands.

---

#### **Implementation Steps**
1. **Software Architecture**:
   - Use an FSM-based approach for both the microcontroller and PC application.
   - Design and diagram the FSMs:
     - **Microcontroller FSM**: Handles commands from the PC and performs corresponding actions.
     - **PC Application FSM**: Processes user input, communicates with the microcontroller, and handles responses.

2. **Microcontroller Implementation**:
   - **Initial State**: Begin in a low-power sleep mode.
   - **Interrupt Handling**:
     - RX Interrupt: Parse received data into commands (for state changes) or data (for ongoing state operations).
     - TX Interrupt: Send requested data back to the PC.
   - **Peripheral Connections**:
     - RGB LED: P2.0, P2.1, P2.2.
     - Buzzer: P2.4.
     - Potentiometer: P1.3.
     - LCD: Data pins (P1.4-P1.7), Control pins (P2.5-P2.7).
   - Implement eight states corresponding to menu options sent by the PC application.

3. **PC Application**:
   - Use a blocking mechanism to listen for incoming data and send responses.
   - Display the main menu and process user input for sending commands to the microcontroller.

4. **Menu Operations**:
   - **State 1**: Cycle RGB LED colors with a user-specified delay \(X\).
   - **State 2**: Increment and display a counter value.
   - **State 3**: Play a sequence of tones on the buzzer.
   - **State 4**: Accept user input to set the delay \(X\).
   - **State 5**: Measure potentiometer voltage dynamically and display on the LCD.
   - **State 6**: Clear LCD and reset the counter.
   - **State 7**: Display the menu.
   - **State 8**: Enter idle (sleep) mode.

5. **Hardware Setup**:
   - Ensure correct wiring of peripherals (LED, buzzer, potentiometer, LCD) as per the pin assignments.
