# PiOS
Simple prototype operating system for the Raspberry Pi. It aims to be a simple operating system for learning and teaching the ins and outs of operating system development in general and the architecture of a real world System on a Chip (BCM2835) in particular.

## Goals
The main goal of the project is to learn loads of interesting facts about operating system development. Maybe it will be possible to use the operating system in OS-classes for teaching and experimenting. 

The operating system will probably not become stable enough to use in real-life. You've been warned.

Maybe this can become a set of drivers and features which can be used as a static library by other embedded programming projects on the Raspberry Pi. They probably all exist already somewhere else, like in the [RaspberryPi-Repository](https://github.com/dwelch67/raspberrypi) of dwelch67.

## "Current" state
The code is currently being updated and rewritten in C. The following drivers are tested and work well:
* GPIO
* UART (AUX)
* Postman requesting a framebuffer from VideoCore IV-GPU (only)

The following device-drivers might not work as expected:
* I2C-interface
* Interface to an LCD character display

I'm thinking about an interface for adding functions to specific interrupt service routines and also about a generic driver interface.
