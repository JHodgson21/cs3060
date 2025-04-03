CC = gcc

all: assn5

assn5: "assn5.c"
	$(CC) "assn5" -o assn5

clean:
	del assn5.exe