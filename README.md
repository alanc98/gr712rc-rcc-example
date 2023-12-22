# GR712RC RTEMS RCC 1.3.1 demo 

This image is intended for the GR712RC development board using rcc-1.3.1

This project is a demo program to try to demonstrate an issue with RTEMS/GR712RC GPIO interrupts.

The obvervation is that a GPIO interrupt seems to cause the processor context to become corrupted leading to a RTEMS "BLOWN STACK" error or an exeption. It is very hard to isolate, but it seems more likely to happen when there are a higher rate of interrupts or context switches combined with GPIO interrupts.

It's hard to quantify, but a GPIO interrupt handler seems to cause this issue. On a system that has many tasks, and is using multiple device drivers (SPW/RMAP, UARTS, SPI, Ethernet, etc) a 1Hz GPIO interrupt seems to cause a crash anywhere from 1 hour up to over 12 hours.
This example program was created by removing device drivers and application code until I had the minimum amount of functionality that could still demonstrate the issue.
 
I have noticed that increasing the rate of the RTEMS tick (to an unrealistic 10,000 hz ) and increasing the GPIO ISR rate to 100Hz cause the problem to occur much faster.

If the RTEMS clock tick rate is low (100Hz) and there is only one or two tasks in addition to the GPIO interrupt/ISR, then the problem may not easily occur.

I found that the best way to get the problem to occur is to icrease the RTEMS clock tick to 10,000Hz, increase the GPIO Interrupt rate to 100Hz, and either run the shell task or run several demo tasks that switch out by the operating system scheduler.
 
To increase task activity, the "demo" tasks are created which just compete to aquire a RTEMS semaphore.
The problem still occurs without the demo tasks running, but it could take longer.

The error manifests itself in a few potential ways:

1. When the "BLOWN STACK" error occurs, the stack that is being reported on does not seem to be corrupted. When we have an image with the shell enabled, we record the locations of all of the stacks and dump them using GRMON after the error occurs and see that the stacks do not look corrupted. It seems to be a corrupt processor context instead.

2. Sometimes an exception can occur (which we catch with GRMON). This can be a trap, illegal instruction, or with a full software build a data access error. In the case of the data access error, a GRMON 'inst' buffer dump shows that a register value becomes loaded with zero instead of a valid SRAM address, causing FLASH memory to be read and the contents used as a pointer.

Note that our production system and variations of the demo system run without crashing or stack blown errors if we do not enable the GPIO ISR.

The GPIO interrupt for this demo is generated by a STM32 microcontroller board programmed to cause the GPIO interrupt at different rates.

To build:
This image has only been built on a Linux system (Ubuntu 20.04)

Make sure the rcc-1.3.1 compiler is in your path.

If the rcc-1.3.1 compiler is not in /opt, please adjust the macros in build/rtems-paths.mak

```
$ cd build/leon-rcc
$ make
```

After the build, the image to load via GRMON is rki.elf.

The RTEMS and DRVMGR configuration options are in build/leon-rcc/config/rtems_config.h

