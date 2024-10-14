CC=g++
CFLAGS=-lOpenGL -lglfw -lGLEW

default:
	make all

all: main.cpp
	$(CC) -o main main.cpp $(CFLAGS)

clean:
	rm -f main.o main
