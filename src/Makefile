# Used to compile all the modules and create the game, the Makefile is the coolest file.

CC=gcc
PATH_ALLEGRO=C:\allegro-5.0.10-mingw-4.7.0
CFLAGS=-O2 -std=c99 -I modules -I $(PATH_ALLEGRO)\include
LDFLAGS=-L $(PATH_ALLEGRO)\lib
LIBS=-lallegro-5.0.10-monolith-mt

SOURCES=$(wildcard modules/*.c) 
HS=$(wildcard modules/*.h) 
OBJECTS=$(patsubst modules/%.c,build/%.o,$(SOURCES))
DEPS=$(patsubst %.o,,$(OBJECTS))

TARGET=game.exe

all: $(TARGET)

dev: CFLAGS=-g -Wall -Iinclude -Wall -Wextra $(OPTFLAGS)

dev: all

$(TARGET): build $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFLAGS) $(LDFLAGS) $(LIBS)

build/%.o: modules/%.c $(HS)
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)

include $(DEPS)

$(DEPS): ;

build:
	mkdir "build"