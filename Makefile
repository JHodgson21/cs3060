CC = gcc

TARGET = assn4
SRC = assn4.c

all: assn4

assn4: "assn4.c"
    gcc -o assn4 assn4.c   

clean:
	del assn4.exe
