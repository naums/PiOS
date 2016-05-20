###############################################################################
#	makefile
#	 by Alex Chadwick
#   see http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/
#
#	A makefile script for generation of raspberry pi kernel images.
###############################################################################

# The toolchain to use. arm-none-eabi works, but there does exist 
# arm-bcm2708-linux-gnueabi.
#ARMGNU ?= $(CC)
ARMGNU ?= arm-none-eabi

#ASOPTS=-march=arm -mcpu=arm1176jzf-s
ASOPTS=-march=armv6 -mcpu=arm1176jzf-s #--mfpu=vfp
LDOPTS=
CFLAGS=-std=c99

# The intermediate directory for compiled object files.
BUILD = build/

# The directory in which source files are stored.
SOURCE = source/

# The name of the output file to generate.
TARGET = kernel.img

# The name of the assembler listing file to generate.
LIST = kernel.list

# The name of the map file to generate.
MAP = kernel.map

# The name of the linker script to use.
LINKER = kernel.ld

# The names of all object files that must be generated. Deduced from the 
# assembly code files in source.
ASSRCS := $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))
LIBCOBJ := $(patsubst $(SOURCE)libc/%.s, $(BUILD)%_libc.o, $(wildcard $(SOURCE)libc/*.s))
CSRCS := $(patsubst $(SOURCE)%.c, $(BUILD)%_c.o, $(wildcard $(SOURCE)*.c))

PROCESS := $(patsubst $(SOURCE)process/%.s,$(BUILD)%_process.o,$(wildcard $(SOURCE)process/*.s))
PROCESSC := $(patsubst $(SOURCE)process/%.c,$(BUILD)%_process_c.o,$(wildcard $(SOURCE)process/*.c))

OBJECTS = $(ASSRCS) $(LIBCOBJ) $(CSRCS) $(PROCESS) $(PROCESSC)

# Rule to make everything.
all: $(TARGET) $(LIST)

# Rule to remake everything. Does not include clean.
rebuild: all

# Rule to make the listing file.
$(LIST) : $(BUILD)output.elf
	$(ARMGNU)-objdump -d $(BUILD)output.elf > $(LIST)

# Rule to make the image file.
$(TARGET) : $(BUILD)output.elf
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET) 

# Rule to make the elf file.
$(BUILD)output.elf : $(OBJECTS) $(LINKER)
	$(ARMGNU)-ld --no-undefined $(OBJECTS) $(LDOPTS) -Map $(MAP) -o $(BUILD)output.elf -T $(LINKER)

# Rule to make the object files.
$(BUILD)%.o: $(SOURCE)%.s $(BUILD)
	$(ARMGNU)-as -I $(SOURCE) $(ASOPTS) $< -o $@
	
$(BUILD)%_libc.o: $(SOURCE)libc/%.s $(BUILD)
	$(ARMGNU)-as -I $(SOURCE) $(ASOPTS) $< -o $@

$(BUILD)%_c.o: $(SOURCE)%.c $(BUILD)
	$(ARMGNU)-gcc -I $(SOURCE) $(CFLAGS)  $< -c -o $@

$(BUILD)%_process_c.o: $(SOURCE)process/%.c $(BUILD)
	$(ARMGNU)-gcc -I $(SOURCE) $(CFLAGS) $< -c -o $@ 
$(BUILD)%_process.o: $(SOURCE)process/%.s $(BUILD)
	$(ARMGNU)-as -I $(SOURCE) $(ASOPTS) $< -c -o $@ 


$(BUILD):
	mkdir $@

# Rule to clean files.
clean : 
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(LIST)
	-rm -f $(MAP)
