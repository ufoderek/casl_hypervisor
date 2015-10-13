# CASL Hypervisor

## Overview

CASL Hypervisor is an ARM-based hypervisor designed for the [Multi-Core Virtual Platform](https://github.com/ufoderek/mvp).

* Supports on-demand memory allocation for VMs.
* Capable of time-sharing VM context-switch.
* Supports virtual and physical interrupts.
* Implementation of virtual and physical-passthrogh devices.
* Capitalizes MVP semi-host for file I/O, enable it for easier development.

## System Requirement

1. Mondern Mac OS X or Linux distributions (tested on OS X El Capitan)
2. ARM cross compiler, which can be obtained via:
    - [Linaro](https://www.linaro.org) (Linux)
    - [GCC ARM embedded](https://launchpad.net/gcc-arm-embedded) (Linux and OS X)

## Build

Make sure the ARM cross compiler (arm-none-eabi-) is in your $PATH.

```
    $ make mt -j4
```