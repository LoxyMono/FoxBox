TARGET := FoxBox.img

EMULATOR := qemu-system-x86_64
EMUARGS  := -monitor stdio -m 4G \
			-serial file:Logs/Serial.log -debugcon file:Logs/Debug.log -drive file=$(TARGET),format=raw,if=ide -d int -D Logs/QEMU.log

.PHONY: default $(TARGET)

default: $(TARGET)

hdd-info: $(TARGET)
	@minfo -i $(TARGET)
	@mdir -i $(TARGET)

run: $(TARGET)
	@mkdir -p Logs
	$(EMULATOR) $(EMUARGS)

$(TARGET):
	@cd Bootloaders/BIOS && make
	@cd Kernel && make
	@dd if=/dev/zero of=$@ bs=1M count=64
	@mkfs.vfat $@ -F 32 -R 16 -b 0 -n "FoxBox" --invariant --mbr=y
	@dd if=Bootloaders/BIOS/Bin/VBR.bin of=$@ conv=notrunc bs=1 count=422 seek=90 skip=90
	@dd if=Bootloaders/BIOS/Bin/VBR.bin of=$@ conv=notrunc bs=512 count=3 seek=2 skip=1
	@mcopy -i $@ Bootloaders/BIOS/Bin/Startup.bin ::
	@mmd -i $@ Kernel Modules
	@mcopy -i $@ Kernel/Bin/FoxKrnl ::
	@mcopy -i $@ -s BootFiles/* ::

clean:
	@cd Bootloaders/BIOS && make clean
	@cd Kernel && make clean
	rm -rf Logs
	rm $(TARGET)