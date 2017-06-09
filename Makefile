# COMPILERS and stuff 
ARM = arm-none-eabi
CC = $(ARM)-gcc
AS = $(ARM)-as
AR = $(ARM)-ar

SHELL := /bin/bash
## find the version number of gcc (I think this is a bit too complicated :/)
CCVERSION:=$(shell $(CC) --version | sed 's/[ ]/\n/g' | grep "\." | xargs | awk '{ print $$1 }')
ARMGCCLIBPATH=/usr/lib/gcc/arm-none-eabi/$(CCVERSION)/

# FOLDERS
SOURCE=source/
BUILD=build/
LIB=lib/
QEMU=build/qemu_


# OPTIONS for AS, CC and LD
# todo: tweak for RPI, RPIB+, RPI2, RPI3
CPU=arm1176jzf-s

INC=include
ASOPTS=-g $(CPUINFO)
LIBS=-lpios -lc -lm -lgcc #-lyailfc
LDOPTS=$(LIBS)
CFLAGS=-std=c99 -Wall -pedantic -g $(CPUINFO) $(CCPU) -I$(INC) -O2 #-mcpu=arm1176jzf-s

LIBEXCLUDE=$(BUILD)main.o\
           $(BUILD)cpu_exception.o\
           $(BUILD)start.o\
           $(BUILD)syscalls.o

QEMUEXCLUDE = $(BUILD)start.o

LIBC=lib/libc.a
LIBM=lib/libm.a
NEWLIB_PATH=newlib-cygwin
NEWLIB=$(LIB)$(NEWLIB_PATH)
NEWLIB_PREP=$(NEWLIB)/newlib/libc/sys/arm/

# KERNELNAME
KRNL=kernel
LDSCRIPT=$(KRNL).ld

# gather all s and c files
OBJECTS := $(filter-out $(QEMU)start.o, $(patsubst $(SOURCE)%.s,$(BUILD)%.o, $(patsubst $(SOURCE)%.c,$(BUILD)%.o, $(wildcard $(SOURCE)*.s) $(wildcard $(SOURCE)*.c))))
LIBOBJ := $(filter-out $(LIBEXCLUDE),$(OBJECTS))
OBJECTS := $(filter-out $(LIBOBJ),$(OBJECTS))

QOBJ := $(filter-out $(QEMUEXCLUDE), $(OBJECTS)) $(QEMU)start.o

all: 
	$(MAKE)  rpi

PLAT=PLATFORM_RPI 

qemu: PLAT=PLATFORM_QEMU
qemu: OBJECTS := $(QOBJ)
qemu: LDSCRIPT = qemu.ld
qemu: CPU=arm926ej-s
qemu: $(BUILD) $(KRNL).img

libqemu: PLAT=PLATFORM_QEMU
libqemu: CPU=arm926ej-s
libqemu: $(BUILD) $(LIB)libpios.a

CPUINFO=-mcpu=$(CPU) -mfpu=vfp #-march=armv6
CCPU=-marm -mfloat-abi=hard

lib: PLAT=PLATFORM_RPI 
lib: $(BUILD) $(LIB)libpios.a
librpi: PLAT=PLATFORM_RPI 
librpi: $(BUILD) $(LIB)libpios.a

librpi2: PLAT=PLATFORM_RPI2 
librpi2: $(BUILD) $(LIB)libpios.a

librpibp: PLAT=PLATFORM_RPIBP 
librpibp: $(BUILD) $(LIB)libpios.a

# RPI
rpi: PLAT=PLATFORM_RPI 
rpi: $(BUILD) $(KRNL).img

# RPI2
rpi2: PLAT=PLATFORM_RPI2 
rpi2: $(BUILD) $(KRNL).img

# RPI B+
rpibp: PLAT=PLATFORM_RPIBP 
rpibp: $(BUILD) $(KRNL).img 

# build newlib
# TARGET_CC="arm-none-eabi-gcc"
# TARGET_AR="arm-none-eabi-ar"
newlib: $(BUILD) $(LIBC)
$(LIB)build/arm-none-eabi/newlib/libc.a: 
	cd $(LIB)build && \
		CFLAGS_FOR_TARGET="$(CPUINFO) $(CCPU)"\
		../$(NEWLIB_PATH)/configure --target=arm-none-eabi --enable-newlib-hw-fp --with-float=hard --with-cpu=arm1176jzf-s --with-fpu=vfp --disable-multilib --disable-shared --enable-target-optspace  --disable-newlib-supplied-syscalls && \
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
	$(ARM)-objdump -d $(KRNL).elf > $(KRNL).list

# make the kernel-image
$(KRNL).img: $(KRNL).elf
	$(ARM)-objcopy $(KRNL).elf -O binary $(KRNL).img
$(KRNL).elf: $(OBJECTS) $(LIB)libpios.a	
	$(ARM)-ld --no-undefined $(OBJECTS) $(LDOPTS) -Map $(KRNL).map -o $(KRNL).elf -L $(LIB) -L $(ARMGCCLIBPATH) -T $(LDSCRIPT)

$(QEMU)%.o: $(SOURCE)qemu_%.c
	$(CC) -D$(PLAT) -I $(SOURCE) $(CFLAGS) $< -c -o $@
$(QEMU)%.o: $(SOURCE)qemu_%.s
	$(AS) -I $(SOURCE) $(ASOPTS) $< -o $@

# build objectfiles from assembler or c
$(BUILD)%.o: $(SOURCE)%.s
	$(AS) -I $(SOURCE) $(ASOPTS) $< -o $@
$(BUILD)%.o: $(SOURCE)%.c
	$(CC) -D$(PLAT) -I $(SOURCE) -I $(LIB)yailfc/src $(CFLAGS) $< -c -o $@

$(LIB)libpios.a: $(LIBOBJ)
	echo $(LIBOBJ)
	$(AR) rcv $(LIB)libpios.a $(LIBOBJ)

# create the build-folder
$(BUILD):
	mkdir $(BUILD)

# delete the created files
clean : 
	-rm $(BUILD)*.o
	-rm $(KRNL).img $(KRNL).elf $(KRNL)
	-rm $(KRNL).list $(KRNL).map
