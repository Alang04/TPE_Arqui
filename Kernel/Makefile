include Makefile.inc

AS := nasm
ASFLAGS := -felf64

CC := x86_64-linux-gnu-gcc
CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-builtin -fno-stack-protector -mno-red-zone -Iinclude -nostdlib 

LD := x86_64-linux-gnu-ld
LDFLAGS := --warn-common -z max-page-size=0x1000 -T kernel.ld

ASM_SRCS := loader.asm asm/libasm.asm
ASM_OBJS := $(ASM_SRCS:.asm=.o)

C_SRCS := $(wildcard c/*.c)
C_OBJS := $(C_SRCS:.c=.o)

OBJS := $(ASM_OBJS) $(C_OBJS)

KERNEL := kernel.bin

.PHONY: all clean

all: $(KERNEL)

# assemble NASM sources
loader.o: loader.asm
	$(AS) $(ASFLAGS) loader.asm -o loader.o

asm/libasm.o: asm/libasm.asm
	$(AS) $(ASFLAGS) asm/libasm.asm -o asm/libasm.o

# compile C sources
c/%.o: c/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# link kernel
$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(KERNEL) $(OBJS)