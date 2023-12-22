# GR712RC RTEMS RCC 1.3.1 demo 

This image is intended for the GR712RC development board using rcc-1.3.1

This project is a demo program to try to demonstrate an issue with RTEMS/GR712RC GPIO issue.

The issue seems to be that a GPIO interrupt can cause the processor context to become corrupted leading to a RTEMS "BLOWN STACK" error or an exeption. It is very hard to isolate, but it seems more likely to happen when there are a higher rate of interrupts or context switches combined with the GPIO interrupts.

I have noticed that increasing the rate of the RTEMS tick (to an unrealistic 10,000 hz ) and increasing the GPIO ISR rate to 100Hz cause the problem to occur much faster.

Also, the problem takes much longer to occur (up to hours) if you lower the clock tick rate, gpio interrupt rate, or there is not much other task activity.
To increase task activity, the "demo" tasks are created which just compete to aquire a RTEMS semaphore.
The problem still occurs without the demo tasks running, but it could take longer.

The error manifests itself in a few potential ways:

1. When the "BLOWN STACK" error occurs, the stack that is being reported on does not seem to be corrupted. When we have an image with the shell enabled, we record the locations of all of the stacks and dump them using GRMON after the error occurs and see that the stacks do not look corrupted. It seems to be a corrupt processor context instead.

2. Sometimes an exception can occur ( which we catch with GRMON ). This can be a trap, illegal instruction, or on our produduction system a data access error.

Note that our production system and all of these demo systems run without crashing or stack blown errors if we do not enable the GPIO ISR.

This image is the result of removing all other application code and drivers until it looked like the GPIO ISR was causing some sort of processor context corruption.

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




