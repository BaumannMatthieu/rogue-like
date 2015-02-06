CC = g++
CFLAGS = -Wall -Wextra -lSDL2 -lSDL2_image
NAME = roguelike
CFILES = $(wildcard *.cpp)
HFILES = $(wildcard *.h)
OBJECTS = $(CFILES:.cpp=.o)

all: prog

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $< $(HFILES)

prog: $(OBJECTS)
	$(CC) $< $(CFLAGS) -o $(NAME)

clean:
	rm *.o $(NAME)
