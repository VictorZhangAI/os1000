QEMU=qemu-system-riscv32
CC=clang
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fno-stack-protector -ffreestanding -nostdlib
OBJCOPY=llvm-objcopy

all:
	$(CC) $(CFLAGS) -Wl,-Tuser.ld -Wl,-Map=shell.map -o shell.elf \
	shell.c \
	user.c \
	common.c \
	virtio.c
	$(OBJCOPY) --set-section-flags .bss=alloc,contents -O binary shell.elf \
	shell.bin
	$(OBJCOPY) -Ibinary -Oelf32-littleriscv shell.bin \
	shell.bin.o
	$(CC) $(CFLAGS) -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
	kernel.c \
	common.c \
	virtio.c \
	shell.bin.o
	$(QEMU) -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
	-d unimp,guest_errors,int,cpu_reset -D qemu.log \
	-drive id=drive0,file=smooth.txt,format=raw,if=none \
	-device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
        -kernel kernel.elf

.PHONY: clean
clean:
	rm *.elf *.map *.o *.log shell.bin 
