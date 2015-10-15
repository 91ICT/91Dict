CC          = gcc
CFLAGS      = `pkg-config --cflags gtk+-3.0 gmodule-2.0`
LIBS        = `pkg-config --libs   gtk+-3.0 gmodule-2.0` -lbt
INC         = ./inc
LIB         = ./lib
SRCS        = 91ict.c  main.c callbacks.c
OBJS        = $(SRCS:.c=.o)
EXECUTABLE  = 91Dict
# DATA          = foldoc-dict.data  foldoc-soundex.data bookmark.data suggests.data
DATA_DIR	= data

all: $(SRCS) $(EXECUTABLE)

debug: CFLAGS += -g -Wall
debug: clean $(SRCS) $(EXECUTABLE)

profiling: CFLAGS += -pg
profiling: all

$(EXECUTABLE): $(OBJS)
	if ! [ -d "./$(DATA_DIR)" ]; then mkdir ./$(DATA_DIR); fi
	$(CC) -I$(INC) $(CFLAGS) $(OBJS) -o $@  -L$(LIB) $(LIBS)

.c.o:
	$(CC) -I$(INC) $(CFLAGS) -c $< -o $@ -L$(LIB) $(LIBS)

clean:
	rm -rf *.o $(EXECUTABLE)
	rm -rf $(DATA_DIR)
	

remake: clean all
reprofiling: clean profiling