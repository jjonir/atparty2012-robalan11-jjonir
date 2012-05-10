CC = gcc
CFLAGS = -Wall -pedantic -ansi
CPPFLAGS = -Wall -pedantic -ansi
LDLIBS = -lGLEW -lglut -lGLU -lGL -lm
OBJECTS = plasma.o tunnel.o cmd.o shaders.o main.o

all: test

test: $(OBJECTS)

clean:
	rm -f *.o test
