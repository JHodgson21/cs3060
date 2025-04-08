CC = gcc

# The executable
TARGET = assn5

# Source file
SRC = assn5.c

# Build target
all: $(TARGET)

# Building executable
$(TARGET): $(SRC)
	$(CC) -pthread -o $(TARGET) $(SRC)

# Cleaning compiled files
clean:
	rm -f $(TARGET)

