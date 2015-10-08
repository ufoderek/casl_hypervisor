# CASL Hypervisor

## Overview

CASL Hypervisor is an ARM-based hypervisor designed for the [Multi-Core Virtual Platform](https://github.com/ufoderek/mvp).

It supports:

* Fixed memory allocation
* Interrupt sharing
* VM context-switch
* Virtual and physical devices for VM

## System Requirement

1. Kubuntu 12.04 64-bit
2. ARM GCC

## Build

Make sure the arm-none-eabi- toolchain is in your $PATH.

```
    $ make mt -j4
```