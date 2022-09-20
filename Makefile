SRC := src
LIB := lib
OBJ := obj

SRC_MODULES  := $(shell find $(SRC)/ -type f -name '*.c')
SRC_HEADERS  := $(shell find $(SRC)/ -type f -name '*.h')

LIB_MODULES  := $(patsubst $(LIB)/%, %, $(shell  find $(LIB)/ -maxdepth 1 -type d))
LIB_HEADERS  := $(shell find $(LIB)/ -type f -name '*.h')
LIB_ARCHIVES := $(patsubst %, $(OBJ)/lib%.a, $(LIB_MODULES))

WARN  := -Wpedantic -Werror -Wimplicit -Wall -Wno-unused-function
STD   := -std=c2x
OPT   := -O0
DEBUG := -ggdb -fsanitize=address
INPUT := in.txt
LINK  := -lm -L$(OBJ) -Wl,--start-group $(addprefix -l, $(LIB_MODULES)) -Wl,--end-group

all: main

test: main $(INPUT)
	./main $(INPUT)

clean:
	$(RM) main obj/*

main: $(SRC_MODULES) $(SRC_HEADERS) $(LIB_ARCHIVES)
	$(CC) $(DEBUG) -o $@ -iquote. $(SRC_MODULES) $(LINK) $(OPT)

$(OBJ)/lib%.a: $(OBJ)/%.o
	ar rcs $@ $<
	ranlib $@

$(OBJ)/%.o: $(LIB)/%/*.c $(LIB_HEADERS)
	$(CC) -c $< -o $@ $(OPT) $(DEBUG) $(WARN) $(STD)
