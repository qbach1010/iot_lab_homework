IoT Lab Homework

Repository: qbach1010/iot_lab_homework

This repository contains a set of STM32-based IoT lab exercises focused on learning embedded programming. Each lab homework was developed using STM32CubeIDE / STM32CubeMX, C language, STM32 Hardware Abstraction Layer (HAL) library and other library avalable that I've found on github.

I just write a short description in this readme file, for more detailed explaination and why I do it that way, you can check out my Google Slides (Vietnamese) that I use to report back to my senior weekly on the progress: https://docs.google.com/presentation/d/1YPuxtM02O0CWDc4GCb5DM86VsOLfvX1zrCVpzRzVggM/edit?usp=sharing

Lab 1: Parity Checker

Goal:

    Read three input buttons and determine whether the number of pressed inputs is odd or even.
    Create a parity function on ARM Cortex Assembly language
    Use the result to switch an onboard LED state.

How it was done:

    Configured GPIO pins PA3, PA4, and PA5 as digital inputs.
    Read the button states in a loop.
    Combined the readings into a binary value.
    Passed the value to a custom parity function write with assembly.
    Used the result to turn the LED on or off with HAL_GPIO_WritePin().

Lab 2: Function Practice and OLED Display

Goal:

    Practice writing reusable C functions.
    Display text on an SSD1306 OLED screen.

How it was done:

    Created an STM32 project and initialized I2C for the OLED module.
    Used SSD1306 driver files (source: https://github.com/afiskon/stm32-ssd1306 ) to send display commands and text.
    Implemented functions such as:
        SayHello()
        Distance()
        OverLap()
    The program continuously printed “Hello!” on the OLED screen and demonstrated simple function-based logic.

Lab 3: LED Brightness Control

Goal:

    Learn about timer-based control and LED brightness modulation.
    Implement blinking and breathing effects.

How it was done:

    Used TIM2 as a timer interrupt source.
    Configured GPIO on the STM32 to control the LED output.
    Created a brightness variable that increased and decreased smoothly for the breathing effect.
    Added button-based logic to switch modes between:
        blinking
        breathing
    Implemented a simple debouncing method using state tracking and timing checks.

Lab 5: Traffic Light Controller with OLED UI

Goal:

    Build a finite-state machine (FSM) simulation for a crossroad traffic light.
    Show traffic light status on an OLED display.

    Noticed that the original lesson want us to use physical led for the traffic light, but I think that using OLED screen would be more intunitive.

How it was done:

    Designed a state machine with phases such as:
        - Go South: South is green, all other road is red
        - Wait South: South is yellow, all other road is red
        - Clear South: All road is red, south is waiting for west/walk light to change
        - Go West: West is green, all other road is red
        - Wait West: West is yellow, all other road is red
        - Clear West: All road is red, west is waiting for south/walk light to change
        - Go Walk: Walk is green, all other road is red
        - Wait Walk: Walk is yellow, all other road is red
        - Clear Walk: All road is red, walk is waiting for west/south light to change
    Read input signals from GPIO pins to determine which traffic direction was active.
    Updated the LED state and timer values according to the current FSM state.
    Used SSD1306 functions to draw the crossroad and show the current light values and remaining time.

Tools and Technologies Used

    STM32 microcontrollers: I use an F401 blackpill board, which in my experience have been far more stable than F103 bluepill for just around a 20k VND (1$) more 
    STM32CubeIDE / STM32CubeMX
    Standard HAL library
    Credit to ssd1306 library by afiskon, which same me a lot of time: https://github.com/afiskon/stm32-ssd1306 

Summary

These labs helped build a strong foundation in embedded systems, from basic digital input/output to timers, state machines, and display-driven user interfaces. The projects are a practical introduction to developing firmware for microcontrollers using the STM32 ecosystem.
