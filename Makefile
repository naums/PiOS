# FOLDERS
SOURCE=source/
BUILD=build/
LIB=lib/

# TODO: copy the correct board.mk to the main-folder
include board.mk

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
LIBS=-lpios -lc -lm -lgcc #-lyailfc
LDOPTS=$(LIBS)
CFLAGS=-std=c99 -Wall -pedantic -g $(CPUINFO) -marm -mfloat-abi=hard -I$(INC) -Os #-mcpu=arm1176jzf-s

# exclude the following file for building the library
LIBEXCLUDE=$(BUILD)main.o\
           $(BUILD)cpu_exception.o\
           $(BUILD)start.o\
           $(BUILD)qemu_start.o\
           $(BUILD)syscalls.o

LIBC=lib/libc.a
LIBM=lib/libm.a
NEWLIB_PATH=newlib-cygwin
NEWLIB=$(LIB)$(NEWLIB_PATH)
NEWLIB_PREP=$(NEWLIB)/newlib/libc/sys/arm/

NEWLIB_CFLAGS?=$(CPUINFO) $(CCPU)
NEWLIB_OPTS?=--target=arm-none-eabi --enable-newlib-hw-fp --with-float=hard --with-cpu=arm1176jzf-s --with-fpu=vfp --disable-multilib --disable-shared --enable-target-optspace  --disable-newlib-supplied-syscalls

# KERNELNAME
KRNL=kernel
LDSCRIPT?=$(KRNL).ld

# gather all s and c files
OBJECTS := $(patsubst $(SOURCE)%.s,$(BUILD)%.o, $(patsubst $(SOURCE)%.c,$(BUILD)%.o, $(wildcard $(SOURCE)*.c))) $(ASFILES)
LIBOBJ := $(filter-out $(LIBEXCLUDE),$(OBJECTS))
OBJECTS := $(filter-out $(LIBOBJ),$(OBJECTS))

all: $(BUILD)  $(KRNL).img
	
lib: $(BUILD) $(LIB)libpios.a

# build newlib
# TARGET_CC="arm-none-eabi-gcc"
# TARGET_AR="arm-none-eabi-ar"
newlib: $(BUILD) $(LIBC)
$(LIB)build/arm-none-eabi/newlib/libc.a: 
	cd $(LIB)build && \
		CFLAGS_FOR_TARGET="$(NEWLIB_CFLAGS)\
		../$(NEWLIB_PATH)/configure $(NEWLIB_OPTS) && \
		$(MAKE) 

$(LIBC): $(LIB)build/arm-none-eabi/newlib/libc.a
	cp $(LIB)build/arm-none-eabi/newlib/libc.a $(LIBC)
	cp $(LIB)build/arm-none-eabi/newlib/libm.a $(LIBM)

newlib_clean:
	rm -r $(LIB)build
	mkdir $(LIB)build
	rm $(LIBC) $(LIBM)

yailfc:
	cd $(LIB)yailfc && \
		CPU="$(CPUINFO) $(CCPU)"\
		PF="arm-none-eabi-" \
		make lib
	mv $(LIB)yailfc/libyailfc.a $(LIB)libyailfc.a

yailfc_clean:
	rm $(LIB)libyailfc.a
	cd $(LIB)yailfc && \
		make clean
		
# make a listing from the kernel.elf file
dump: $(BUILD) $(KRNL).elf $(KRNL).list
$(KRNL).list : $(KRNL).elf
	$(OBJDUMP) -d $(KRNL).elf > $(KRNL).list

# make the kernel-image
$(KRNL).img: $(KRNL).elf
	$(OBJCOPY) $(KRNL).elf -O binary $(KRNL).img
	
$(KRNL).elf: $(OBJECTS) $(LIB)libpios.a	$(LIBC)
	$(LD) --no-undefined $(OBJECTS) $(LDOPTS) -Map $(KRNL).map -o $(KRNL).elf -L $(LIB) -L $(ARMGCCLIBPATH) -T $(LDSCRIPT)

# build objectfiles from assembler or c
$(BUILD)%.o: $(SOURCE)%.s
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
