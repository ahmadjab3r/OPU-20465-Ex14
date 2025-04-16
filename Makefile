# -------------------------------
# Compiler and Flags
# -------------------------------
CC      := gcc
CFLAGS  := -Wall -ansi -pedantic -std=c90
EXEC    := assembler

# -------------------------------
# Source and Header Files
# -------------------------------
HDR_DIR := Header Files

SRC_FILES := \
	assembler.c \
	first_run.c \
	initial_run.c \
	second_run.c \
	hash_table.c \
	linked_list.c \
	utilities.c

HDR_FILES := \
	$(HDR_DIR)/first_run.h \
	$(HDR_DIR)/initial_run.h \
	$(HDR_DIR)/second_run.h \
	$(HDR_DIR)/hash_table.h \
	$(HDR_DIR)/linked_list.h \
	$(HDR_DIR)/utilities.h \
	$(HDR_DIR)/skeleton.h

# -------------------------------
# Object Files
# -------------------------------
OBJ_FILES := $(SRC_FILES:.c=.o)

# -------------------------------
# Default Target
# -------------------------------
all: $(EXEC)

# -------------------------------
# Linking the Executable
# -------------------------------
$(EXEC): $(OBJ_FILES)
	@echo "Linking executable..."
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES)

# -------------------------------
# Compiling Source Files
# -------------------------------
%.o: %.c $(HDR_FILES)
	@echo "Compiling $< ..."
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------
# Clean Build Files
# -------------------------------
clean:
	@echo "Cleaning up..."
	rm -f $(OBJ_FILES) $(EXEC)

# -------------------------------
# Rebuild Everything
# -------------------------------
rebuild: clean all

# -------------------------------
# Run Executable
# -------------------------------
run: all
	@echo "Running..."
	./$(EXEC)

# -------------------------------
# Phony Targets
# -------------------------------
.PHONY: all clean rebuild run