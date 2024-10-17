
all:  bootloader kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

ej1.bin: ej1.asm
	nasm -f bin -o ej1.bin ej1.asm




.PHONY: bootloader image collections kernel userland all clean
