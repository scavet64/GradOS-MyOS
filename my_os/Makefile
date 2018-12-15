CC=gcc
TARGET=myos
BIN=./bin/
C_SOURCES = $(wildcard kernel/*.c drivers/*.c libc/string/*.c libc/conversion/*.c system/*.c *.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h include/*.h system/*.h)
OBJS=$(C_SOURCES:.c=.o)

all compile: $(TARGET)
all: finale
.PHONY: all compile clean finale

%.o: %.c ${HEADERS}
	$(CC) -c $(@:.o=.c) -o $@ -ffreestanding -fno-exceptions -m32 -O0

$(TARGET): $(OBJS)
	$(shell nasm -f elf start.asm -o start.o)
	$(CC) -m32 -nostdlib -nodefaultlibs -lgcc start.o $? -T linker.ld -o $(TARGET)

finale:
	$(shell cd /media/sf_VMShare/git/my_os/)
	$(shell cp $(TARGET) ./iso/boot/$(TARGET))
	$(shell grub2-mkrescue iso --output=$(TARGET).iso)
	$(shell cp $(TARGET).iso /media/sf_VMShare/$(TARGET).iso)

clean:
	rm -f *.o $(TARGET) $(TARGET).iso
	find . -name \*.o | xargs --no-run-if-empty rm
