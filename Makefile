# Compiler and flags
CC = clang
BPF_CFLAGS = -O2 -target bpf -I/usr/include/bpf
BPF_OBJECT = simple_xdp_program.o

# Source file
SRC = simple_xdp_program.c

# Default target
all: $(BPF_OBJECT)

# Compile the BPF program
$(BPF_OBJECT): $(SRC)
	$(CC) $(BPF_CFLAGS) -c $(SRC) -o $(BPF_OBJECT)

# Clean up generated files
clean:
	rm -f $(BPF_OBJECT)

.PHONY: all clean
