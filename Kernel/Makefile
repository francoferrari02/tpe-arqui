include Makefile.inc

KERNEL = kernel.bin
SOURCES = $(wildcard *.c) $(wildcard IDT/*.c)
SOURCES_ASM = $(wildcard asm/*.asm)
OBJECTS = $(SOURCES:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)
LOADERSRC = loader.asm
LOADEROBJECT = $(LOADERSRC:.asm=.o)
STATICLIBS =

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

# Compilación de archivos .c a .o
%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

# Compilación de archivos .asm a .o
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin

.PHONY: all clean

# include Makefile.inc

# KERNEL = kernel.bin
# SOURCES = $(wildcard .c) 
# SOURCES_IDT=$(wildcard IDT/.c)
# SOURCES_ASM = $(wildcard asm/.asm)


# OBJECTS = $(SOURCES:.c=.o)
# OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
# OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

# LOADERSRC = loader.asm
# LOADEROBJECT = $(LOADERSRC:.asm=.o)
# STATICLIBS =
# ALL_OBJECTS= $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_IDT) 

# all: $(KERNEL)
# $(KERNEL): $(STATICLIBS) $(ALL_OBJECTS)
# 	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

# # Compilación de archivos .c a .o
# %.o: %.c
# 	$(GCC) $(GCCFLAGS) -I./include -I./IDT -c $< -o $@

# # Compilación de archivos .asm a .o
# %.o: %.asm
# 	$(ASM) $(ASMFLAGS) $< -o $@

# $(LOADEROBJECT):
# 	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# clean:
# 	rm -rf asm/*.o *.o *.bin

# .PHONY: all clean



# LOADERSRC=loader.asm

# LOADEROBJECT=$(LOADERSRC:.asm=.o)
# STATICLIBS=
# ALL_OBJECTS= $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPT) $(OBJECTS_DRIVERS)

# all: $(KERNEL)
# $(KERNEL): $(STATICLIBS) $(ALL_OBJECTS)
#     $(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

# %.o: %.c
#     $(GCC) $(GCCFLAGS) -I./include -I./interruptions -c $< -o $@
# %.o : %.asm
#     $(ASM) $(ASMFLAGS) $< -o $@

# $(LOADEROBJECT):
#     $(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# clean:
#     rm -rf $(ALL_OBJECTS) *.bin

# .PHONY: all clean
