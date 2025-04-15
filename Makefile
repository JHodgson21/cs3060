CC = gcc

TARGET = assn7

SRC = assn7.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -pthread -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
