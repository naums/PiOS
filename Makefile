# COMPILERS and stuff 
ARM = arm-none-eabi
CC = $(ARM)-gcc
AS = $(ARM)-as

# FOLDERS
SOURCE=source/
BUILD=build/
LIB=lib/


# OPTIONS for AS, CC and LD
# todo: tweak for RPI, RPIB+, RPI2, RPI3
ASOPTS=-g -march=armv6 -g -mfloat-abi=softfp -mhard-float -mfpu=vfp -marm -Os #-mcpu=arm1176jzf-s
LDOPTS=
CFLAGS=-std=c99 -Wall -pedantic -g -march=armv6 -g -mfloat-abi=softfp -mhard-float -mfpu=vfp -marm -Os #-mcpu=arm1176jzf-s

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
	make rpi
	
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
newlib: $(BUILD) $(LIB)newlib.a
$(LIBC): $(NEWLIB_PREP)syscalls.c.orig $(NEWLIB_PREP)libcfunc.c.orig $(NEWLIB_PREP)crt0.S.orig
	touch $(NEWLIB_PREP)syscalls.c $(NEWLIB_PREP)libcfunc.c $(NEWLIB_PREP)crt0.S
	cd $(LIB)build &&\
		CC=$(CC) \
		CFLAGS=$(CFLAGS) \
		$(NEWLIB_PATH)/configure --target=arm-none-eabi --host=x86_64-pc-linux-gnu --disable-multilibs --disable-shared
	make -C ../$(NEWLIB_PATH)

# move some files away, so newlib won't use them
$(NEWLIB_PREP)%.orig:
	mv $< $@

# make a listing from the kernel.elf file
dump: $(BUILD) $(KRNL).elf $(KRNL).list
$(KRNL).list : $(KRNL).elf
	$(ARM)-objdump -d $(KRNL).elf > $(KRNL).list

# make the kernel-image
$(KRNL).img : $(KRNL).elf
	$(ARM)-objcopy $(KRNL).elf -O binary $(KRNL).img
$(KRNL).elf : $(OBJECTS) $(LINKER)
	$(ARM)-ld --no-undefined $(OBJECTS) $(LDOPTS) -Map $(KRNL).map -o $(KRNL).elf -T $(KRNL).ld

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
