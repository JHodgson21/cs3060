CC = gcc

#the executable
TARGET = assn4

#source file
SRC = assn4.c

#build target
all: assn4

#building executable
assn4: "assn4.c"
    gcc -o assn4 assn4.c   

#cleaning compiled files
clean:
	rm -f assn4.exe
