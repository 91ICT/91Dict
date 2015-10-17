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

all: $(SRCS) $(EXECUTABLE) init_data

debug: CFLAGS += -g -Wall
debug: clean $(SRCS) $(EXECUTABLE)

profiling: CFLAGS += -pg
profiling: all

$(EXECUTABLE): $(OBJS)
	@if ! [ -d "./$(DATA_DIR)" ]; then mkdir ./$(DATA_DIR); fi
	@$(CC) -I$(INC) $(CFLAGS) $(OBJS) -o $@  -L$(LIB) $(LIBS)

.c.o:
	@$(CC) -I$(INC) $(CFLAGS) -c $< -o $@ -L$(LIB) $(LIBS)

init_data:
	@$(CC) $(CFLAGS) -c init_data.c -I$(INC) -L$(LIB) $(LIBS)
	@$(CC) -I$(INC) $(CFLAGS) 91ict.o init_data.o -o init_data -L$(LIB) $(LIBS)
	@echo "Begin create data"
	@./init_data
	@echo "Create data done"
	@echo "Clean init file !"
	@rm init_data init_data.o
	@echo "Done ready to use"

clean:
	@echo "Clean Data"
	@rm -rf *.o $(EXECUTABLE)
	@rm -rf $(DATA_DIR)
	@echo "Clean Data Done"

remake: clean all
	
reprofiling: clean profiling