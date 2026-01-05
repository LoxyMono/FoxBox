ASM := nasm
CC  := i686-elf-gcc
LD  := i686-elf-ld

CC_FLAGS := -Wall -Wextra \
			-ffreestanding \
			-m32 -IInclude/C -nostdlib -O0

LD_FLAGS := -T Config/Linker.ld -m elf_i386

ASM_SRC := $(shell find Source/ASM -name "*.asm")
ASM_OBJ := $(ASM_SRC:Source/ASM/%.asm=Objs/ASM/%.o)

C_SRC := $(shell find Source/C -name "*.c")
C_OBJ := $(C_SRC:Source/C/%.c=Objs/C/%.o)

OBJS  := $(filter-out Objs/ASM/VBR.o, $(ASM_OBJ)) $(C_OBJ)

TARGET := Bin/VBR.bin Bin/Startup.bin

default: $(TARGET)

Bin/VBR.bin: Source/ASM/VBR.asm
	@mkdir -p $(dir $@)
	$(ASM) -f bin $^ -o $@

Bin/Startup.bin: $(OBJS)
	@mkdir -p $(dir $@)
	$(LD) $(LD_FLAGS) -o $@ $^

Objs/ASM/%.o: Source/ASM/%.asm
	@mkdir -p $(dir $@)
	$(ASM) -f elf $< -o $@

Objs/C/%.o: Source/C/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	rm -rf Bin Objs
	rm -f $(TARGET)