CXX := i686-elf-g++
ASM := nasm

CXX_FLAGS := -std=c++17 -Wall -Wextra \
			 -ffreestanding -fno-exceptions -fno-builtin -fno-rtti -fno-stack-protector \
			 -mno-red-zone -m32 -g -MMD -MP -IInclude/CPP \
			 -nostdinc -nostdlib

ASM_FLAGS := -f elf
LD_FLAGS  := -T Config/Linker.ld -static -nostdlib -z max-page-size=0x1000

CXX_SRC := $(shell find Source/CPP -name "*.cpp")
ASM_SRC := $(shell find Source/ASM -name "*.asm")
OBJS    := $(CXX_SRC:Source/CPP/%.cpp=Objs/CPP/%.o) $(ASM_SRC:Souce/ASM/%.asm=Objs/ASM/%.o)
DEPS    := $(CXX_SRC:Source/CPP/%.cpp=Objs/CPP_D/%.o)

TARGET := Bin/FoxKrnl

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(LD_FLAGS) -o $@ $^

-include $(DEPS)
Objs/CPP/%.o: Source/CPP/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

Objs/ASM/%.o: Source/ASM/%.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASM_FLAGS) $< -o $@

clean:
	rm -rf Bin Objs