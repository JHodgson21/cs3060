CC = gcc

all: assn6

assn5: "assn6.c"
	$(CC) "assn6" -o assn6

clean:
	del assn6.exe