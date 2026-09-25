# Smart Exam Hall Monitoring and Management System

An ARM7-based embedded system built using the **LPC2148** to automate examination timing, monitor room temperature, provide visual status indication, and handle examination interruptions.

## Overview

The **Smart Exam Hall Monitoring and Management System** integrates multiple LPC2148 peripherals into a single embedded application. The system provides real-time examination timing, temperature monitoring, countdown display, pause/resume control, and examination completion indication.

This project was developed as a practical application to understand and implement different peripherals of the LPC2148 microcontroller.

## Features

- Real-time date and time display using the **RTC**
- Examination duration configuration using a **4×4 matrix keypad**
- Examination countdown timer
- Remaining examination time displayed on **two multiplexed 7-segment displays**
- Room temperature monitoring using **LM35 + ADC**
- Temperature display on the **16×2 LCD**
- Three-level examination status indication:
  - Green LED — more than 70% of examination time remaining
  - Yellow LED — between 30% and 70% remaining
  - Red LED — 30% or less remaining
- **Pause/Resume** functionality using an external interrupt
- Automatic recording of examination **start and end times**
- Buzzer indication when the examination reaches zero
- LCD-based status and configuration interface

## Hardware Used

- LPC2148 ARM7TDMI microcontroller
- 16×2 LCD
- 4×4 Matrix Keypad
- LM35 Temperature Sensor
- 10-bit ADC
- RTC
- Two 7-segment displays
- LEDs
  - Green
  - Yellow
  - Red
- Buzzer
- Push switches
- USB-UART / DB-9 interface

## LPC2148 Peripherals Used

- **GPIO**
- **RTC**
- **ADC**
- **External Interrupts (EINT0/EINT1)**
- **VIC (Vectored Interrupt Controller)**
- **LCD interfacing**
- **Keypad interfacing**
- **Timer/delay functions**
- **7-segment multiplexing**

## System Operation

### 1. Normal Operation

After powering the system, the LCD displays the current RTC time and room temperature.

```text
Time: 12:30:45
Temp: 28°C

The LM35 produces an analog voltage proportional to temperature. The LPC2148 ADC converts this voltage into a digital value, which is then converted into temperature.

2. Examination Configuration

The administrator enters the configuration mode using the keypad.

The system allows:

RTC date/time configuration
Examination start time configuration
Examination duration configuration
3. Examination Start

Once the configured examination time is reached, the system:

Records the examination start time using the RTC.
Starts the examination countdown.
Displays the remaining time on the multiplexed 7-segment display.
Updates the LED status according to the remaining time.
4. Countdown Indication

The two 7-segment displays show the remaining examination time in minutes:

30 → 29 → 28 → ... → 02 → 01 → 00

The displays are multiplexed using shared segment lines.

5. LED Status

The LEDs provide a quick visual indication of the remaining examination time.

Remaining Time	Indication
More than 70% of exam duration	Green
More than 30% and up to 70%	Yellow
30% or less	Red
00 minutes	Buzzer
6. Pause / Resume

An external interrupt is used for pause/resume functionality.

When the pause switch is pressed:

Running → Paused

Pressing it again:

Paused → Running

The countdown resumes from the remaining time.

7. Examination Completion

When the countdown reaches 00:

The buzzer is activated.
The examination is marked as completed.
The RTC is used to record the examination end time.
The relevant information is displayed on the LCD.
Important Calculations
ADC Resolution

## Application

The project demonstrates how multiple microcontroller peripherals can be integrated into an examination-management application to provide automated timing, environmental monitoring, status indication, and interruption handling.

> **Note:** This project was developed as a practical LPC2148 peripheral-integration exercise, with the objective of understanding and implementing the individual hardware peripherals through a single application.

## Author

**Pravin Patil**

Embedded Systems / ARM7 Project
