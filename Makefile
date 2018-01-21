# FOLDERS
SOURCE=source/
BUILD=build/
LIB=lib/

BOARD=qemu
include boards/$(BOARD)/board.mk

# COMPILERS and stuff 
PLATFORM ?= arm-none-eabi
CC = $(PLATFORM)-gcc
AS = $(PLATFORM)-as
AR = $(PLATFORM)-ar
LD = $(PLATFORM)-ld
OBJCOPY = $(PLATFORM)-objcopy
OBJDUMP = $(PLATFORM)-objdump

SHELL := /bin/bash
## find the version number of gcc (I think this is a bit too complicated :/)
CCVERSION:=$(shell $(CC) --version | sed 's/[ ]/\n/g' | grep "\." | xargs | awk '{ print $$1 }')
ARMGCCLIBPATH=/usr/lib/gcc/$(PLATFORM)/$(CCVERSION)/

# OPTIONS for AS, CC and LD
CPU?=arm1176jzf-s
INC=include
CPUINFO=-mcpu=$(CPU) -mfpu=vfp #-march=armv6
ASOPTS=-g $(CPUINFO)
LIBS=-lgcc
LDOPTS=$(LIBS)
CFLAGS=-std=c99 -Wall -pedantic -g $(CPUINFO) -marm -mfloat-abi=hard -I$(INC) -Iboards/$(BOARD)/ -Os #-mcpu=arm1176jzf-s

# KERNELNAME
KRNL=kernel
LDSCRIPT?=$(KRNL).ld

# gather all s and c files
ASFILES += $(BUILD)process_low_s.o $(BUILD)string_s.o $(BUILD)stdio_s.o
OBJECTS := $(patsubst $(SOURCE)%.s,$(BUILD)%_s.o, $(patsubst $(SOURCE)%.c,$(BUILD)%.o, $(wildcard $(SOURCE)*.c))) $(ASFILES)
#LIBOBJ := $(filter-out $(LIBEXCLUDE),$(OBJECTS))
#OBJECTS := $(filter-out $(LIBOBJ),$(OBJECTS))

all: $(BUILD)  $(KRNL).img
	
lib: $(BUILD) #$(LIB)libpios.a

# make a listing from the kernel.elf file
dump: $(BUILD) $(KRNL).elf $(KRNL).list
$(KRNL).list : $(KRNL).elf
	$(OBJDUMP) -d $(KRNL).elf > $(KRNL).list

# make the kernel-image
$(KRNL).img: $(KRNL).elf
	$(OBJCOPY) $(KRNL).elf -O binary $(KRNL).img
	
$(KRNL).elf: $(OBJECTS)
	$(LD) --no-undefined $(OBJECTS) $(LDOPTS) -Map $(KRNL).map -o $(KRNL).elf -L $(LIB) -L $(ARMGCCLIBPATH) -T boards/$(BOARD)/kernel.ld

# build objectfiles from assembler or c
$(BUILD)%_s.o: $(SOURCE)%.s
	$(AS) -I $(SOURCE) $(ASOPTS) $< -o $@
$(BUILD)%.o: $(SOURCE)%.c
	$(CC) -I $(SOURCE) -I $(LIB)yailfc/src -I. $(CFLAGS) $< -c -o $@

$(LIB)libpios.a: $(LIBOBJ)
	$(AR) rcv $(LIB)libpios.a $(LIBOBJ)

# create the build-folder
$(BUILD):
	mkdir $(BUILD)

# delete the created files
clean : 
	-rm $(BUILD)*.o
	-rm $(KRNL).img $(KRNL).elf $(KRNL)
	-rm $(KRNL).list $(KRNL).map
