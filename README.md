# AVR-Atmega32-Security-System
An embedded security system for access control, featuring EEPROM-based password storage, door operation mechanics, and alarm activation upon multiple failed entries. Designed for 2 microcontrollers with UART, I2C, and motor control capabilities.

## Project Overview
This repository contains the code for an embedded security system designed for access control through password management. The system interacts with users to set and verify passwords, controls a door mechanism, and activates an alarm after multiple failed access attempts.

## Features
- **Password Setting and Verification**: Users can input a password that is then stored in EEPROM.
- **Door Control**: On successful password entry, the system operates a door via a DC motor, allowing or denying access.
- **Alarm Activation**: If the password is entered incorrectly four times in a row, the system triggers an alarm.

## Components
- 2 Microcontrollers
- External EEPROM
- DC Motor for door operation
- Buzzer for alarm functionality
- LCD
- KEYPAD

## DRIVERS
- GPIO
- External EEPROM
- DC Motor 
- Buzzer 
- LCD
- KEYPAD
- UART
- I2C
- TIMER
- PWM

# Project Structure:
## First  Microcontroller HMI ECU
1. Application Layer (APP)
  - **HMI_ECU.c**: The main application file that orchestrates the interaction between different modules and controls the overall system behavior For HMI Fucnctions.
  -  **hmi_functions.c/h**: the implementation of functions for the Human-Machine Interface (HMI) of an Electronic Control Unit (ECU). It includes initialization, password handling, command sending and receiving, and user interface interactions.
    
2. Hardware Abstraction Layer (HAL)
  - **LCD.C/h**: Facilitates communication with the LCD display to numbers and results.
  - **KEYPAD.c/h**: Contains functions for initializing and controlling the keypad to take inputs from the user.
    
3. Microcontroller Abstraction Layer (MCAL)
  - **GPIO.c/h**: Manages the MCU PINS and PORTS and the communcation between any external device.
  - **uart.c/h**: Implements the UART communication driver for the communication between the two microcontrollers.
  - **timer.c/h**: Implements the timer1 driver to count time.

## Second  Microcontroller CONTROL ECU
1. Application Layer (APP)
  - **CONTROL_ECU.c**: Main file for the Control Electronic Control Unit (ECU). It initializes the system, checks for existing passwords, and continuously processes incoming commands related to password management and door operations.
  -  **control_functions.c/h**: the implementation of control functions for the Control ECU.
    It includes initialization of system components, password handling, and door operation logic.
    
2. Hardware Abstraction Layer (HAL)
  - **eeprom.c/h**: Used to save the password.
  - **buzzer.c/h**: Provides functionality for controlling the buzzer and generating audible feedback when the alarm triggerd.
  - **dc-motor.c/h**: Provides functionality for controlling the dc motor and generating movment simulation for door like openign and closing the door.
    
3. Microcontroller Abstraction Layer (MCAL)
  - **GPIO.c/h**: Manages the MCU PINS and PORTS and the communcation between any external device.
  - **uart.c/h**: Implements the UART communication driver for the communication between the two microcontrollers.
  - **timer.c/h**: Implements the timer1 driver to count time.
  - **pwm.c/h**: Provides PWM signal generation for controlling the dc motor.
  - **i2c.c/h**: Implements the i2c (twi) communication driver for the communication between the control ECU and the External EEPROM.

## How to Use
1. Clone the repository to your local machine.
2. Compile the project using an AVR development environment (e.g., Atmel Studio or eclipse ide).
3. Flash the compiled code to the 2 microcontrollers.
4. Interact with the system via the HMI to set and verify passwords.
5. You can run the simulation circuit using proteus simulation app.

#### Note:
- Ensure proper wiring and connections between the microcontroller and hardware components.
- Adjust system parameters as needed for optimal performance and accuracy.
- Refer to the provided source code and documentation for detailed implementation and customization instructions.
