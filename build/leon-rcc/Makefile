##
## RTEMS demo makefile
##

##
## Architecture
##
ARCH ?= sparc-gaisler-rtems5

##
## Select your BSP here 
##
# BSP ?= gr740_smp
# BSP ?= leon3
BSP ?= gr712rc
# BSP ?= leon3_sf

##
## paths for the RTEMS tools and RTEMS BSP
##
include ../rtems-paths.mak

##
## Compiler paths
##
PREFIX         = $(RTEMS_TOOL_BASE)
RTEMS_PREFIX   = $(RTEMS_BSP_BASE)
RTEMS_ARCH_LIB = $(RTEMS_PREFIX)/$(ARCH)/$(BSP)/lib

##
## Linker flags
##
LDFLAGS ?= -mcpu=leon3 -mfix-gr712rc -ffunction-sections -fdata-sections -Wl,--gc-sections
LDFLAGS += --pipe -B$(RTEMS_ARCH_LIB) -qrtems -specs bsp_specs $(WARNINGS)

##
## Compiler Architecture Switches
##
ARCH_OPTS ?= -mcpu=leon3 -mfix-gr712rc -ffunction-sections -fdata-sections
ARCH_OPTS += --pipe -B$(RTEMS_ARCH_LIB) -qrtems -specs bsp_specs

INCLUDE_PATH := -I./config

WARNINGS	= -Wall
DEBUG_OPTS	 = -g -O2

##
## define build products
##
EXE_TARGET       = rki.elf
BINARY_TARGET    = rki.bin

##
## Objects to build
##
OBJS = rki_init.o rki_drvmgr.o
OBJS += watchdog.o 
OBJS += gpio_demo.o
OBJS += demo_tasks.o

##
## Libraries to link
##
## Start Group needed in RCC
LIBS := --start-group
LIBS += -lrtemscpu
LIBS += -lrtemsbsp
LIBS += -latomic
LIBS += -lc
LIBS += -lm
LIBS += -lgcc

## End Group needed in RCC
LIBS += --end-group

##
## Extra Cflags for Assembly listings, etc.
##
LIST_OPTS    = -Wa,-a=$*.lis

##
## General gcc options that apply to compiling and dependency generation.
##
COPTS=$(LIST_OPTS) $(ARCH_OPTS) $(WARNINGS) $(INCLUDE_PATH)  -I.

##
## Extra defines and switches for assembly code
##
ASOPTS = -P -xassembler-with-cpp

####################################################
## Host Development System and Toolchain defintions
##
## Host OS utils
##
RM=rm -f
CP=cp
MV=mv
CD=cd
TAR=tar
CAT=cat
MKDIR=mkdir
LS=ls

##
## Compiler tools
##
COMPILER   = $(ARCH)-gcc
ASSEMBLER  = $(ARCH)-gcc
LINKER	   = $(ARCH)-ld
AR	   = $(ARCH)-ar
NM         = $(ARCH)-nm
OBJCOPY    = $(ARCH)-objcopy
SIZE       = $(ARCH)-size

##
## VPATH
##
VPATH := ./config
VPATH += ../../src

###############################################################################################
##
## Build Targets
##

##
## The default "make" target is the subsystem object module.
##
default::$(EXE_TARGET)

##
## Compiler rule
##
.c.o:
	$(COMPILER)  $(COPTS) $(DEBUG_OPTS)  -c -o $@ $<

##
## Assembly Code Rule
##
.s.o:
	$(COMPILER) $(ASOPTS) $(COPTS) $(DEBUG_OPTS)  -c -o $@ $<

##
## Link Rule to make the final executable image
## add symtab.o for symbol table
##
$(EXE_TARGET): $(OBJS) 
	$(COMPILER) $(DEBUG_FLAGS) $(LDFLAGS) -o $(EXE_TARGET) $(OBJS) $(LIBS)
	$(OBJCOPY) -O binary --strip-all $(EXE_TARGET) $(BINARY_TARGET)
	$(SIZE) $(EXE_TARGET)

##
## Make clean rule
##
clean::
	-$(RM) $(OBJS) $(EXE_TARGET) $(EXE_TARGET_PRE) $(BINARY_TARGET) $(BINARY_TARGET).gz
	-$(RM) *.lis
	-$(RM) *.img

