# greesy
#### George's Really Easy Embedded SYnthesiser

A synthesiser written for the Cortex M4F based STM32F3Discovery, which has the STM32F303VC chip.

I decided against using any libraries or prewritten HALs as I wanted to explore the different ways of abstracting hardware access in C++, and gain a deeper understanding of the underlying hardware.

Uses a sine wave sample with a hardcoded envelope to generate sound. Accepts MIDI note-on and note-off signals.

#### Video below - click through to YouTube - excuse the rubbish keyboard playing :)

[![Video](https://img.youtube.com/vi/hqPZdQX19pI/maxresdefault.jpg)](https://youtu.be/hqPZdQX19pI)

#### Installation

Requires an STM32F3discovery board, a MIDI breakout board, and an audio jack.
The MIDI input should be connected to GPIO pin `B7`, and audio output should be connected to GPIO pin `A4`.
Download and compile using CMake:
```
cd greesy/
mkdir build && cd build
cmake ..
make
```

You can then connect the board to your computer and flash it using 

```st-flash write main.bin 0x08000000```

#### Usage

Connect a MIDI keyboard to your MIDI input and play!

#### How it works
##### Sound generation
A sine wave sample and an amplitude envelope is hardcoded in an array in `tables.hpp`.
Using a floating point index and linear interpolation, these are indexed into roughly every 22μs (by a timer interrupt).
The result from the amplitude envelope is then applied to the result from the sine wave table and is sent to the DAC (Digital-Analogue Converter) peripheral which outputs it as audio.

#### MIDI Input
The MIDI input is connected to the USART1 peripheral of the board.
Whenever data is received to this peripheral, an interrupt is used to write it to a (large) circular buffer.
The main loop then checks for any pending MIDI instructions in this circular buffer, and if any are present sends them to the synthesiser module.

