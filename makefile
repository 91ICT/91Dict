CC     = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0 gmodule-2.0`
LIBS   = `pkg-config --libs   gtk+-3.0 gmodule-2.0`
DEBUG  = -Wall -g
INC = ./inc
LIB = ./lib
LFLAGS = -lbt
OBJECTS = main.o callbacks.o

.PHONY: clean

all: 91Dict

91Dict: $(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o $@ $(LIBS) -I$(INC) -L$(LIB) $(LFLAGS)

main.o: main.c support.h
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@ -I$(INC) -L$(LIB) $(LFLAGS)

callbacks.o: callbacks.c support.h
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@ -I$(INC) -L$(LIB) $(LFLAGS)

clean:
	rm -f *.o 91Dict