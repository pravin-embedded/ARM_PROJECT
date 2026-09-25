# Smart Exam Hall Monitoring and Management System

An ARM7-based embedded system built on the **LPC2148** (ARM7TDMI) that automates
examination timing, monitors hall temperature, provides visual status indication,
and handles examination interruptions.

All content below is derived from the source code committed in this repository.

---

## Hardware / Platform

| Item | Detail |
|---|---|
| MCU | NXP **LPC2148**, ARM7TDMI core |
| Device header | `lpc21xx.h` (Keil LPC2000 device header) |
| Oscillator | 12 MHz (`FOSC` in `include/ADC_defines.h`), `CCLK = 60 MHz`, `PCLK = 15 MHz` |
| Toolchain | ARM Compiler (`__irq` interrupt keyword), Keil uVision target |
| RTC crystal | 32.768 kHz |

> The Keil project file (`.uvprojx` / `.uvoptx`) and all build outputs
> (`.obj`, `.axf`, `.hex`, `.map`) are intentionally **not** committed.

---

## Repository Structure

```
ARM_PROJECT/
|-- README.md
|-- src/
|   |-- Project.c             main() - top-level control loop
|   |-- Project_defines.c     hardware init, screens, countdown, LEDs, buzzer, admin menu
|   |-- adc_defines.c         ADC init + single conversion read
|   |-- delay.c               software delays (US / MS / S)
|   |-- interrupt.c           EINT0 and EINT1 interrupt service routines
|   |-- keypad_defines.c      4x4 keypad scanning + numeric/password entry
|   |-- lcd_defines.c         16x2 LCD driver
|   `-- LM35.c                LM35 temperature read (Celsius / Fahrenheit)
|-- include/
|   |-- headerfile.h          common includes, pin macros, extern globals
|   |-- type.h                u8/s8/u32/s32/f32/cs8 typedefs
|   |-- ADC_defines.h         ADC clock, SFR and channel bit definitions
|   |-- delay.h
|   |-- interrupt.h
|   |-- keypadfunc.h
|   |-- lcd.h
|   |-- LM35.h
|   |-- Project_defines.h     prototypes of the project-level task functions
|   `-- rtc.h                 RTC_Init() prototype
`-- tests/
    `-- rtclcd.c              standalone RTC initialisation module (RTC_Init)
```

### 1. Real-Time Clock (RTC)

`tests/rtclcd.c` provides `RTC_Init()`, which resets the RTC (`CCR`), loads
`PREINT` / `PREFRAC` so the 32.768 kHz crystal is divided down to a 1 Hz time
base, and then enables the RTC. The program reads `DOM`, `MONTH`, `YEAR`,
`HOUR`, `MIN` and `SEC` directly as register values.

On power-up, `RTC_Time_Setup()` prompts on the LCD and accepts the following
from the keypad: `Enter Date`, `Enter Month`, `Enter Year`, `Enter Hour`,
`Enter Min`, `Enter Sec`.

### 2. 4x4 Matrix Keypad

`src/keypad_defines.c` scans a 4x4 matrix (rows on P1.16-P1.19, columns on
P1.20-P1.23) using `row()`, `colcheck()` and `keyscan()`, with this lookup table:

```
1 2 3 /
4 5 6 *
7 8 9 -
C 0 = +
```

Two input helpers are built on top of it:

- `ReadNum()` - builds a number digit by digit, `=` confirms, `C` backspaces.
- `ReadPassword()` - echoes `*` per digit, `=` confirms, `C` backspaces, `*` aborts.

### 3. 16x2 LCD

`src/lcd_defines.c` implements an 8-bit LCD driver (`LCD_Init`, `LCD_Cmd`,
`LCD_Char`, `LCD_str`, `LCD_U32`, `LCD_F32`, `LCD_2Digit`, `LCD_Num`) on
P0.0-P0.7, initialised with `0x38`, `0x0F`, `0x01`, `0x06`.

The LCD runs two alternating screens, switched once per RTC second:

- **Screen 0** - `Time: HH:MM:SS` on line 1 and `Temp:` followed by the
  temperature value, the `0xDF` degree glyph and `C` on line 2.
- **Screen 1** - `Date: DD/MM/YY` on line 1 and the exam state on line 2
  (`Exam:` with minutes remaining, `Start: HH:MM:SS`, or `Ready`).

Screen 0 is shown for 10 s and screen 1 for 3 s before alternating.

### 4. LM35 Temperature Sensing via ADC

`src/adc_defines.c` configures the LPC2148 10-bit ADC (`ADC_Init`) and performs
a single conversion (`ADC_Read`) on channel 3, converting the result to volts
against a 3.3 V reference.

`src/LM35.c` provides `Read_LM35degC()`, which takes 20 averaged samples with a
2 ms delay between them, and `Read_LM35degf()`, which converts the result to
Fahrenheit.

### 5. 2-Digit Multiplexed 7-Segment Display

`Display_2Digit()` in `src/Project_defines.c` drives two common-anode digits:
segments on P0.17-P0.23, digit enables on P0.10 and P0.25, using the segment
lookup table `0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90`.
Each digit is enabled, held by a busy-wait delay, and disabled before the next
digit is loaded, so both digits appear lit together.

`Countdown_Update()` calls `Display_2Digit(mux_time)` while the exam is running,
and otherwise blanks the segment lines and both digit enables.

### 6. External Interrupts

Two edge-triggered external interrupts are configured through the VIC in
`Hardware_Init()` and handled in `src/interrupt.c`:

| Interrupt | Handler | Behaviour |
|---|---|---|
| **EINT0** (P0.0) | `EINT0_isr` | Sets `current_mode` and `admin_mode` to enter the admin menu |
| **EINT1** (P0.14) | `EINT1_isr` | Toggles `pause_flag` (pause / resume) and re-syncs `old_min = MIN` |

### 7. Green / Yellow / Red LED Status Indication

`Exam_Over_Check()` in `src/Project_defines.c` drives three status LEDs
(Green P0.27, Yellow P0.28, Red P0.29) based on the percentage of the exam
duration still remaining:

| Remaining time | LED |
|---|---|
| `exam_time > 70 %` of `exam_time_max` | **Green** |
| `exam_time > 30 %` of `exam_time_max` | **Yellow** |
| otherwise | **Red** |

### 8. Buzzer

The buzzer is on P1.24 (`BUZZER`). It is switched on when the countdown reaches
zero and the system is in normal mode, and is cleared on every other path of
`Exam_Over_Check()`.

### 9. Exam Countdown

- The admin sets a start time (`HH:MM:SS`) and a duration in minutes.
- On reaching the configured RTC time the exam auto-starts, prints
  `Exam Started`, and records the start time.
- `exam_time` is decremented once per RTC minute change (`MIN != old_min`), and
  only while the exam is not paused.
- `mux_time` holds the value shown on the 7-segment display. It is capped at 60
  and reset to the remaining `exam_time` when it reaches zero.

### 10. Exam Start / End Logging

- `start_hour`, `start_min` and `start_sec` are captured when the exam starts.
- `end_hour`, `end_min` and `end_sec` are captured from the RTC when the exam ends.
- `Show_Result()` displays `S: HH:MM` and `E: HH:MM` after the exam for 6 s, then
  resets `exam_time`, `exam_time_max` and `mux_time`.
- The same log is available from the admin menu under `1.View Log`. When no exam
  has been recorded the LCD shows `No Log Found`.

### 11. Pause / Resume

Pressing the EINT1 switch toggles `pause_flag`. While paused, `Pause_Display()`
prints `PAUSE` on the LCD and `Normal_Display()` returns early, so the time,
temperature and countdown updates are held. Pressing again resumes from the
remaining time, because `old_min` is re-synchronised to the current `MIN` in the
interrupt handler.

### 12. Admin / Password Configuration

The admin menu is entered through the EINT0 interrupt. The password is a 32-bit
value defined in `src/Project_defines.c` as `password = 1234`, and it is entered
masked with `*` characters. A wrong password prints `Access Denied`.

Menu options:

- `1.Set Exam` - prompts for `Start Hr`, `Start Min` and `Start Sec` (validated
  against the current RTC time, otherwise `Invalid Time`), then for `Duration`
  in minutes.
- `2.More` - `1.View Log` (start/end log) or `2.Back`.

### 13. Exam Completion

When `exam_time` reaches zero with an exam configured and the system is in normal
mode, the program:

- activates the buzzer,
- displays `EXAM OVER` for 2 s,
- records the exam end time from the RTC,
- clears the configured start time, and then shows the start/end log.

### 14. Software Delays

`src/delay.c` provides busy-wait `delay_US`, `delay_MS` and `delay_S` routines
built on the `type.h` typedefs.

---

## Main Program Flow

`src/Project.c`:

```c
Hardware_Init();      // LCD, ADC, RTC, keypad, VIC/EINT, 7-segment, LEDs, buzzer
RTC_Time_Setup();     // keypad entry of date and time into the RTC

while (1) {
    Countdown_Update();   // multiplex the 7-segment display, or blank it
    Pause_Display();      // show PAUSE when paused
    Normal_Display();     // time / temperature / date / exam screens
    Exam_Over_Check();    // LED thresholds, buzzer, end-of-exam handling
    Show_Result();        // show the start/end log
    Admin_Menu();         // handle admin requests raised by EINT0
}
```

---

## Note on Files Referenced but Not Supplied

The following headers are `#include`d by the committed source but were **not**
present in the supplied project folder, so they have not been added (no files
were invented):

- `Project.h` - included by `src/Project_defines.c` and `include/headerfile.h`
- `ADC.h` - included by `src/LM35.c` and `include/headerfile.h`

In addition, no separate standalone `RTC.c` test program exists; the RTC
initialisation code supplied is `tests/rtclcd.c`.

