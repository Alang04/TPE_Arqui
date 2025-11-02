include Makefile.inc

MODULE=0000-sampleCodeModule.bin
SAMPLE_DATA=0001-sampleDataModule.bin
SOURCES_ROOT=$(wildcard *.c)
SOURCES_C=$(wildcard c/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES_ROOT:.c=.o) $(SOURCES_C:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE) sampleDataModule

$(MODULE): $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o $(MODULE) $(OBJECTS) $(OBJECTS_ASM)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I. -Ic/include -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

sampleDataModule:
	printf "This is sample data." > $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	rm -f asm/*.o c/*.o *.o $(MODULE) $(SAMPLE_DATA)

.PHONY: sampleCodeModule all clean