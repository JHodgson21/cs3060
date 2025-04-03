CC = gcc

# The executable
TARGET = assn6

# Source file
SRC = assn6.c

# Build target
all: $(TARGET)

# Building executable
$(TARGET): $(SRC)
	$(CC) -pthread -o $(TARGET).exe $(SRC)

# Cleaning compiled files
clean:
	rm -f $(TARGET).exe
