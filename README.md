# greesy
#### George's Really Easy Embedded SYnthesiser

A simple synthesiser written for the Cortex M4F based STM32F3Discovery, which has the STM32F303VC chip.

My first embedded project, apart from simple blinky programs. 
I decided against using any libraries or prewritten HALs as I wanted to explore the different ways of abstracting hardware access in C++.
As a result, the peripheral code is a bit varied. I also only started adding tests late in the project.

Uses a single sine wave sample with a hardcoded envelope to generate sound. Accepts MIDI note-on and note-off signals.

#### Video below - click through to YouTube - excuse the rubbish keyboard playing :)

[![Video](https://img.youtube.com/vi/hqPZdQX19pI/maxresdefault.jpg)](https://youtu.be/hqPZdQX19pI)
