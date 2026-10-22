# IoT Lab Homework

A collection of STM32-based IoT lab exercises for learning embedded programming. Each lab was developed with **STM32CubeIDE / STM32CubeMX**, **C**, the **STM32 HAL library**, and a few open-source libraries found on GitHub.

> This README is only a short summary. For more detailed explanations and the reasoning behind my design choices, see my weekly progress report slides (Vietnamese):
> [Google Slides](https://docs.google.com/presentation/d/1YPuxtM02O0CWDc4GCb5DM86VsOLfvX1zrCVpzRzVggM/edit?usp=sharing)

## Table of Contents

- [Lab 1: Parity Checker](#lab-1-parity-checker)
- [Lab 2: Function Practice and OLED Display](#lab-2-function-practice-and-oled-display)
- [Lab 3: LED Brightness Control](#lab-3-led-brightness-control)
- [Lab 5: Traffic Light Controller with OLED UI](#lab-5-traffic-light-controller-with-oled-ui)
- [Tools and Technologies](#tools-and-technologies)
- [Summary](#summary)

---

## Lab 1: Parity Checker

### Goal

- Read three input buttons and determine whether the number of pressed inputs is odd or even.
- Write the parity function in ARM Cortex assembly.
- Use the result to switch an onboard LED.

### How it was done

1. Configured GPIO pins `PA3`, `PA4`, and `PA5` as digital inputs.
2. Read the button states in a loop.
3. Combined the readings into a single binary value.
4. Passed the value to a custom parity function written in assembly.
5. Used the result to turn the LED on or off with `HAL_GPIO_WritePin()`.

---

## Lab 2: Function Practice and OLED Display

### Goal

- Practice writing reusable C functions.
- Display text on an SSD1306 OLED screen.

### How it was done

1. Created an STM32 project and initialized I2C for the OLED module.
2. Used the [afiskon/stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306) driver to send display commands and text.
3. Implemented the following functions:
   - `SayHello()`
   - `Distance()`
   - `OverLap()`

The program continuously prints "Hello!" on the OLED screen and demonstrates simple function-based logic.

---

## Lab 3: LED Brightness Control

### Goal

- Learn timer-based control and LED brightness modulation.
- Implement blinking and breathing effects.

### How it was done

1. Used `TIM2` as a timer interrupt source.
2. Configured a GPIO pin to drive the LED output.
3. Created a brightness variable that rises and falls smoothly to produce the breathing effect.
4. Added button-based logic to switch between **blinking** and **breathing** modes.
5. Implemented simple debouncing using state tracking and timing checks.

---

## Lab 5: Traffic Light Controller with OLED UI

### Goal

- Build a finite-state machine (FSM) simulation of a crossroad traffic light.
- Show the traffic light status on an OLED display.

> **Note:** The original lesson calls for physical LEDs for the traffic lights, but I chose an OLED screen because I think it is more intuitive.

### How it was done

Designed a state machine with the following phases:

| State | Description |
| --- | --- |
| **Go South** | South is green, all other roads are red |
| **Wait South** | South is yellow, all other roads are red |
| **Clear South** | All roads are red; waiting for the west/walk light to change |
| **Go West** | West is green, all other roads are red |
| **Wait West** | West is yellow, all other roads are red |
| **Clear West** | All roads are red; waiting for the south/walk light to change |
| **Go Walk** | Walk is green, all other roads are red |
| **Wait Walk** | Walk is yellow, all other roads are red |
| **Clear Walk** | All roads are red; waiting for the west/south light to change |

Implementation:

1. Read input signals from GPIO pins to determine which traffic direction is active.
2. Updated the light state and timer values according to the current FSM state.
3. Used SSD1306 functions to draw the crossroad and show the current light values and remaining time.

---

## Tools and Technologies

- **Microcontroller:** STM32F401 "Black Pill" board. In my experience it has been far more stable than the F103 "Blue Pill" for only about 20k VND (~$1) more.
- **IDE / configuration:** STM32CubeIDE and STM32CubeMX
- **Library:** STM32 standard HAL
- **Display driver:** [stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306) by afiskon, which saved me a lot of time. Thanks!

---

## Summary

These labs build a strong foundation in embedded systems, covering everything from basic digital I/O to timers, state machines, and display-driven user interfaces. Together they serve as a practical introduction to firmware development on the STM32 ecosystem.
