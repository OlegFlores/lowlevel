TARGET = hello
CC = gcc
CFLAGS = -pedantic \
		-Wall -Werror \
		-s -Os -no-pie \
		-nostdlib \
		-ffreestanding \
		-fno-stack-protector \
		-fdata-sections \
		-ffunction-sections \
		-fno-unwind-tables \
		-fno-asynchronous-unwind-tables \
		-Wl,-n \
		-Wl,--gc-sections \
		-Wl,--build-id=none
# -pedantic							Issue all the warnings demanded by strict ISO C
# -Wall								Show all the warnings about constructions that some users consider questionable
# -Werror							Make all warnings into errors
# -s								Remove all symbol table and relocation information from the executable.
# -Os								Optimize for size. -Os enables all -O2 optimizations except those that often increase code size
# -no-pie							No position independent code
# -nostdlib							Do not use the standard system startup files or libraries when linking
# -ffreestanding					Assert that compilation targets a freestanding environment. This implies -fno-builtin.
#									A freestanding environment is one in which the standard library may not exist, 
#									and program startup may not necessarily be at main
# -fno-stack-protector				Do not emit extra code to check for buffer overflows, such as stack smashing attacks
# -fdata-sections					Place each data item into its own section in the output file if the target supports arbitrary sections
# -ffunction-sections				Place each function into its own section in the output file if the target supports arbitrary sections
# -fno-unwind-tables				Do not generate any needed static data
# -fno-asynchronous-unwind-tables	Do not generate unwind table in DWARF format
# -Wl,option						Pass option as an option to the linker
# -Wl,-n							Set the text segment to be read only, and mark the output as NMAGIC if possible.
# -Wl,--gc-sections					Perform a garbage collection of code and data never referenced. 
# -Wl,--build-id=none				Request creation of .note.gnu.build-id ELF note section. 
#									The contents of the note are unique bits identifying this linked file
# USED DOCUMENTS:
# - gcc documentation: https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html#Option-Summary
# - GNU linker documentation: https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_node/ld_3.html
# - http://sourceware.org/binutils/docs-2.23.1/ld/Options.html#Options
LDFLAGS = -pedantic \
		-Wall -Werror \
		-s -Os -no-pie \
		-nostdlib \
		-ffreestanding \
		-fno-stack-protector \
		-fdata-sections \
		-ffunction-sections \
		-fno-unwind-tables \
		-fno-asynchronous-unwind-tables \
		-Wl,-n \
		-Wl,--gc-sections \
		-Wl,--build-id=none

SRCS := $(wildcard *.c *.S)
OBJDIR = .build
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

DEPDIR = .dep
DEPS := $(SRCS:%.c=$(DEPDIR)/%.d)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

.PHONY: clean run
TARGET = nolibc1

$(OBJDIR)/%.o: %.c | $(OBJDIR) $(DEPDIR)
	@echo [CC] $@
	@$(CC) -c $(CFLAGS) $(DEPFLAGS) -o $@ $<

$(TARGET): $(OBJS)
	@echo [LD] $@
	@$(CC) $(LDFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	@rm -rf $(OBJDIR) $(DEPDIR) $(TARGET)

$(OBJDIR) $(DEPDIR):
	@mkdir -p $@

$(DEPFILES):

include $(wildcard $(DEPFILES))
