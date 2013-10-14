CC      = gcc
OBJCOPY = objcopy
CFLAGS  = -O2 -ffixed-r8 -ffreestanding -Wall -Werror -nostdinc \
          -I ../uboot/include -I include
ASFLAGS = -nostdinc -I ../uboot/include -I include
LDFLAGS = -nostdlib -Wl,-N,-Ttext,$(LOAD_ADDR)

LOAD_ADDR = 0xa3000000

STUBS = ../uboot/stubs.o

objects = kernel.o c_swi_handler.o exit.o swi_handler.o pushing_arg.o read.o write.o exit.o restore_r8.o

default: kernel.bin

.PHONY: default clean clobber

kernel.bin: kernel
	$(OBJCOPY) -O binary $< $@

kernel: start.o $(objects)
	$(CC) $(LDFLAGS) -o $@ $^ $(STUBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f kernel start.o $(objects)

clobber: clean
	rm -f kernel.bin
