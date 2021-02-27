# greesy
#### George's Really Easy Embedded SYnthesiser

A simple synthesiser written for the Cortex M4F based STM32F3Discovery, which has the STM32F303VC chip.

I decided against using any libraries or prewritten HALs as I wanted to explore the different ways of abstracting hardware access in C++, and gain a deeper understanding of the underlying hardware.

Uses a single sine wave sample with a hardcoded envelope to generate sound. Accepts MIDI note-on and note-off signals.

#### Video below - click through to YouTube - excuse the rubbish keyboard playing :)

[![Video](https://img.youtube.com/vi/hqPZdQX19pI/maxresdefault.jpg)](https://youtu.be/hqPZdQX19pI)
