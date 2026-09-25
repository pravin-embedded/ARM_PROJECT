# Smart Exam Hall Monitoring and Management System

An ARM7-based embedded system built using the **LPC2148** to automate examination timing, monitor room temperature, provide visual status indication, and handle examination interruptions.

## Overview

The **Smart Exam Hall Monitoring and Management System** integrates multiple LPC2148 peripherals into a single embedded application. The system provides real-time examination timing, temperature monitoring, countdown display, pause/resume control, and examination completion indication.

This project was developed as a practical application to understand and implement different peripherals of the LPC2148 microcontroller.

## Features

* Real-time date and time display using the **RTC**
* Examination duration configuration using a **4×4 matrix keypad**
* Examination countdown timer
* Remaining examination time displayed on **two multiplexed 7-segment displays**
* Room temperature monitoring using **LM35 + ADC**
* Temperature display on the **16×2 LCD**
* Three-level examination status indication:

  * Green LED — more than 10 minutes remaining
  * Yellow LED — final 10 minutes
  * Red LED — final 1 minute
* **Pause/Resume** functionality using an external interrupt
* Automatic recording of examination **start and end times**
* Buzzer indication when the examination reaches zero
* LCD-based status and configuration interface

## Hardware Used

* LPC2148 ARM7TDMI microcontroller
* 16×2 LCD
* 4×4 Matrix Keypad
* LM35 Temperature Sensor
* 10-bit ADC
* RTC
* Two 7-segment displays
* LEDs

  * Green
  * Yellow
  * Red
* Buzzer
* Push switches
* USB-UART / DB-9 interface

## LPC2148 Peripherals Used

* **GPIO**
* **RTC**
* **ADC**
* **External Interrupts (EINT0/EINT1)**
* **VIC (Vectored Interrupt Controller)**
* **LCD interfacing**
* **Keypad interfacing**
* **Timer/delay functions**
* **7-segment multiplexing**

## System Operation

### 1. Normal Operation

After powering the system, the LCD displays the current RTC time and room temperature.

```text
Time: 12:30:45
Temp: 28°C
```

The LM35 produces an analog voltage proportional to temperature. The LPC2148 ADC converts this voltage into a digital value, which is then converted into temperature.

### 2. Examination Configuration

The administrator enters the configuration mode using the keypad.

The system allows:

* RTC date/time configuration
* Examination duration configuration

### 3. Examination Start

Once the examination begins, the system:

1. Records the examination start time using the RTC.
2. Starts the examination countdown.
3. Displays the remaining time on the multiplexed 7-segment display.
4. Updates the LED status according to the remaining time.

### 4. Countdown Indication

The two 7-segment displays show the remaining examination time in minutes:

```text
30 → 29 → 28 → ... → 02 → 01 → 00
```

The displays are multiplexed using shared segment lines.

### 5. LED Status

The LEDs provide a quick visual indication of the remaining examination time.

| Remaining Time       | Indication |
| -------------------- | ---------- |
| More than 10 minutes | Green      |
| Final 10 minutes     | Yellow     |
| Final 1 minute       | Red        |
| 00 minutes           | Buzzer     |

### 6. Pause / Resume

An external interrupt is used for pause/resume functionality.

When the pause switch is pressed:

```text
Running → Paused
```

Pressing it again:

```text
Paused → Running
```

The countdown resumes from the same remaining time.

### 7. Examination Completion

When the countdown reaches `00`:

* The buzzer is activated.
* The examination is marked as completed.
* The RTC is used to record the examination end time.
* The relevant information can be displayed on the LCD.

## Important Calculations

### ADC Resolution

The LPC2148 ADC used in the project is 10-bit:

$$
2^{10}=1024
$$

Therefore, the ADC output range is:

```text
0 – 1023
```

For a 3.3 V input range:

$$
V_{ADC}=\frac{ADC\ Value\times3.3}{1024}
$$

### LM35 Temperature

The LM35 provides approximately:

$$
10mV/^\circ C
$$

Therefore:

$$
Temperature=\frac{V_{ADC}}{0.01}
$$

or:

$$
Temperature=V_{ADC}\times100
$$

For example, at approximately 28°C:

```text
LM35 output ≈ 0.28 V

Temperature = 0.28 × 100
            ≈ 28°C
```

### RTC

The RTC uses a **32.768 kHz crystal**:

$$
32768=2^{15}
$$

This allows the clock frequency to be divided down to a 1 Hz time base for real-time seconds.

## 7-Segment Multiplexing

The seven segment lines are shared between the two digits.

The system rapidly switches between the two digits:

```text
Enable Digit 1 → Display tens digit
Enable Digit 2 → Display units digit
Repeat rapidly
```

Due to persistence of vision, both digits appear continuously illuminated.

For a common-anode display, the segment logic is active-low.

Example digit patterns:

```text
0 → 0xC0
1 → 0xF9
2 → 0xA4
3 → 0xB0
4 → 0x99
5 → 0x92
6 → 0x82
7 → 0xF8
8 → 0x80
9 → 0x90
```

## Project Structure

A typical source structure is:

```text
Smart-Exam-Hall-Monitoring/
│
├── Project_defines.c
├── headerfile.h
├── lcd.h
├── lcd_defines.c
├── ADC.c
├── RTC.c
├── Keypad.c
├── Interrupt.c
└── README.md
```

The exact files may vary depending on the project version and Keil project organization.

## Development Environment

* **Microcontroller:** LPC2148
* **Core:** ARM7TDMI
* **Programming Language:** Embedded C
* **IDE:** Keil µVision
* **Programming/Flash Tool:** Flash Magic
* **Simulation/Testing:** Proteus, where applicable

## Learning Outcomes

This project provides practical understanding of:

* LPC2148 GPIO configuration
* RTC-based timekeeping
* ADC interfacing
* Analog sensor interfacing
* Matrix keypad scanning
* LCD interfacing
* External interrupt handling
* VIC interrupt configuration
* 7-segment multiplexing
* Countdown implementation
* Embedded timing techniques
* Peripheral integration into a single application

## Application

The project demonstrates how multiple microcontroller peripherals can be integrated into an examination-management application to provide automated timing, environmental monitoring, status indication, and interruption handling.

> **Note:** This project was developed as a practical LPC2148 peripheral-integration exercise, with the objective of understanding and implementing the individual hardware peripherals through a single application.
