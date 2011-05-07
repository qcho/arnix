# Binary Name
BINARY := bin/kernel.bin

CFLAGS := -fno-builtin

# Directories belonging to the project
PROJDIRS := src include
# All source files of the project
CSRCS := $(shell find -L $(PROJDIRS) -type f -name "*.c")
# All header files of the project
CHDRS := $(shell find -L $(PROJDIRS) -type f -name "*.h")
# All asm files of the project
ASMSRCS := $(shell find -L $(PROJDIRS) -type f -name "*.asm")

# All object files in the library
COBJS := $(patsubst src/%.c,bin/%.o,$(CSRCS))
ASMOBJS := $(patsubst src/%.asm,bin/%.o,$(ASMSRCS))

.PHONY: all clean

all: $(BINARY)

$(BINARY): $(ASMOBJS) $(COBJS)
	$(LD) -T link.ld -o $(BINARY) $(ASMOBJS) $(COBJS)

clean:
	-@$(RM) $(wildcard $(BINARY) $(ASMOBJS) $(COBJS))

bin/kstart.o:
	nasm -f aout src/kstart.asm -o bin/kstart.o
	nasm -f aout src/loader.asm -o bin/kstart.o

$(ASMOBJS): bin/%.o : src/%.asm
	nasm -f aout $< -o $@

$(COBJS): bin/%.o : src/%.c
	gcc $(CFLAGS) -MMD -MP -c $< -o $@
