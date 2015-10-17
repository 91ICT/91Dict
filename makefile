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

all: $(SRCS) $(EXECUTABLE) init

debug: CFLAGS += -g -Wall
debug: clean $(SRCS) $(EXECUTABLE)

profiling: CFLAGS += -pg
profiling: all

$(EXECUTABLE): $(OBJS)
	@echo "Executable file"
	@if ! [ -d "./$(DATA_DIR)" ]; then mkdir ./$(DATA_DIR); fi
	@$(CC) -I$(INC) $(CFLAGS) $(OBJS) -o $@  -L$(LIB) $(LIBS)

.c.o:
	@$(CC) -I$(INC) $(CFLAGS) -c $< -o $@ -L$(LIB) $(LIBS)

init:
	@echo "Make init file"
	@if [ -f "./init_data" ]; then rm init_data ; fi
	@$(CC) -c init_data.c -I$(INC) -L$(LIB) -lbt
	@$(CC) -I$(INC)  init_data.o -o init_data -L$(LIB) -lbt
	@echo "Begin create data"
	@./init_data
	@echo "Create data done"
	@rm init_data.o
	@echo "Ready to use"

clean:
	@echo "Clean Data"
	@if [ -f "./init_data" ]; then rm init_data ; fi
	@if [ -f "./$(EXECUTABLE)" ]; then rm -rf *.o $(EXECUTABLE); fi
	@if [ -d "./$(DATA_DIR)" ]; then rm -rf ./$(DATA_DIR) ; fi
	@echo "Clean Data Done"

remake: clean all
	
reprofiling: clean profiling