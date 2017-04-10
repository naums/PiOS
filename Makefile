# COMPILERS and stuff 
ARM = arm-none-eabi
CC = $(ARM)-gcc
AS = $(ARM)-as

ARMGCCLIBPATH=/usr/lib/gcc/arm-none-eabi/6.3.1/

# FOLDERS
SOURCE=source/
BUILD=build/
LIB=lib/


# OPTIONS for AS, CC and LD
# todo: tweak for RPI, RPIB+, RPI2, RPI3
CPU=arm1176jzf-s
CPUINFO=-mcpu=$(CPU) #-mfpu=vfp #-march=armv6
CCPU=-marm #-mfloat-abi=softfp -mhard-float

ASOPTS=-g $(CPUINFO)
LDOPTS=
CFLAGS=-std=c99 -Wall -pedantic -g $(CPUINFO) $(CCPU) -Os #-mcpu=arm1176jzf-s


LIBC=lib/libc.a
LIBM=lib/libm.a
NEWLIB_PATH=newlib-cygwin
NEWLIB=$(LIB)$(NEWLIB_PATH)
NEWLIB_PREP=$(NEWLIB)/newlib/libc/sys/arm/

# KERNELNAME
KRNL=kernel

# gather all s and c files
OBJECTS := $(patsubst $(SOURCE)%.s,$(BUILD)%.o, $(patsubst $(SOURCE)%.c,$(BUILD)%.o, $(wildcard $(SOURCE)*.s) $(wildcard $(SOURCE)*.c)))

# Rule to make the RPI1-version.
all: 
	$(MAKE)  rpi
	
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
newlib: $(BUILD) $(LIB)libc.a
$(LIB)build/arm-none-eabi/newlib/libc.a: 
	cd $(LIB)build && \
		../$(NEWLIB_PATH)/configure --target=arm-none-eabi --host=x86_64-pc-linux-gnu --disable-multilib --disable-shared --enable-target-optspace --enable-newlib-hw-fp --with-float=hard --with-cpu=arm1176jzf-s --with-fpu=vfp --disable-newlib-supplied-syscalls && \
		$(MAKE) 

$(LIBC):
	cp $(LIB)build/arm-none-eabi/newlib/libc.a $(LIBC)
	cp $(LIB)build/arm-none-eabi/newlib/libm.a $(LIBM)

newlib_clean:
	rm -r $(LIB)build
	mkdir $(LIB)build

# make a listing from the kernel.elf file
dump: $(BUILD) $(KRNL).elf $(KRNL).list
$(KRNL).list : $(KRNL).elf
	$(ARM)-objdump -d $(KRNL).elf > $(KRNL).list

# make the kernel-image
$(KRNL).img : $(KRNL).elf
	$(ARM)-objcopy $(KRNL).elf -O binary $(KRNL).img
$(KRNL).elf : $(OBJECTS) $(LINKER)
	$(ARM)-ld --no-undefined $(OBJECTS) $(LDOPTS) -Map $(KRNL).map -o $(KRNL).elf -L $(LIB) -lc -lm -L $(ARMGCCLIBPATH) -lgcc -T $(KRNL).ld

# built objectfiles from assembler or c
$(BUILD)%.o: $(SOURCE)%.s
	$(AS) -I $(SOURCE) $(ASOPTS) $< -o $@
$(BUILD)%.o: $(SOURCE)%.c
	$(CC) -D$(PLAT) -I $(SOURCE) $(CFLAGS)  $< -c -o $@

# create the build-folder
$(BUILD):
	mkdir $(BUILD)

# delete the created files
clean : 
	rm $(BUILD)*.o
	rm $(KRNL).img $(KRNL).elf $(KRNL)
	rm $(KRNL).list $(KRNL).map
