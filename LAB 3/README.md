# Lab 3 - Direct Memory Access (DMA)  

## Overview  
This folder contains the files for **Lab 3**, focusing on the **DMA (Direct Memory Access)** module in MSP430 microcontrollers. This lab emphasizes the efficiency of data transfer operations using DMA instead of the CPU, enabling faster, low-power operations while offloading repetitive data manipulation tasks from the processor.  

---

## Practical Objectives  

### Key Concepts:  
- **DMA Basics**: Understanding the advantages and trade-offs of using DMA over the CPU for data transfers.  
- **MSP430 DMA Module**: Configuring the DMA channels to handle memory-to-memory and peripheral-to-memory transfers.  
- **Hardware-Accelerated Processing**: Using DMA to manipulate and process data structures efficiently.  
- **Interrupt-Driven Design**: Handling user interactions through external interrupts and FSM transitions.  

### DMA Usage Highlights:  
1. Transfer large data blocks without CPU intervention.  
2. Optimize energy consumption by leveraging **low-power modes**.  
3. Understand and implement **DMA priorities** and transfer timing.  
4. Enable **generic and portable** software architecture using layers.  

---

## Functional Requirements  

### State Descriptions:  
The system operates as an **FSM (Finite State Machine)**, transitioning between states based on external button inputs (`PB0–PB2`).  

#### **State 0 (Idle)**  
- The system enters a low-power sleep mode.  
- Transitions out of the idle state upon user input via buttons.  

#### **State 1 (User Input)**  
- Prompt the user to enter a custom idiom (up to 32 characters) via the **Keypad**.  
- Display the entered idiom on the LCD in real-time.  
- Store the idiom in the `recorder_idiom` array.  

#### **State 2 (String Merge)**  
- Request two row indices (0–9) from the user via the **Keypad**.  
- Use DMA to merge the two corresponding rows from the `data_matrix` into the `strMerge` buffer.  
- Display the merged string across two lines on the LCD, with scrolling capability.  
- End the state after displaying the merged string.  

#### **State 3 (LED Animation)**  
- Use DMA to cyclically display the array `{128, 64, 32, 16, 8, 4, 23, 13, 40}` on LEDs, triggered by **TimerB**.  
- Each LED update occurs every 500 ms, with data transferred block by block using DMA.  
- End the state upon user input for a state change.  

---

## Software Architecture  

The system is structured into **portable layers**:  
1. **BSP Layer**: Hardware configurations for GPIO, TimerB, Keypad, DMA, and LCD.  
2. **HAL Layer**: Drivers for peripherals (e.g., Keypad, LCD, DMA transfer functions).  
3. **API Layer**: High-level abstractions for user interactions and FSM state handling.  
4. **Application Layer**: FSM logic for transitioning between states and managing DMA operations.  

---

## Files in this Folder  

   - `main.c`: FSM implementation and system logic.  
   - `bsp.c/h`: Hardware-specific configurations.  
   - `hal.c/h`: Peripheral drivers, including DMA, LCD, and Keypad support.  
   - `api.c/h`: Functions for user input handling and high-level DMA operations.  

---

## How to Run  

1. Connect the hardware as described in the **Hardware Setup** section.  
2. Compile and upload the provided code to the microcontroller using **CCS (Code Composer Studio)**.  
3. Interact with the system using buttons (`PB0–PB2`) and the **Keypad**.  
4. Verify outputs on the LCD and LEDs.  

 
