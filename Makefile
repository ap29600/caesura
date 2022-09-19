LIB := lib
OBJ := obj

LIB_SOURCES  := $(wildcard $(LIB)/*.c)
LIB_OBJECTS  := $(patsubst $(LIB)/%.c, $(OBJ)/%.o,    $(LIB_SOURCES))
LIB_ARCHIVES := $(patsubst $(LIB)/%.c, $(OBJ)/lib%.a, $(LIB_SOURCES))
LIB_NAMES    := $(patsubst $(LIB)/%.c, -l%,           $(LIB_SOURCES))

WARN  := -Wpedantic -Werror -Wimplicit -Wall -Wno-unused-function
STD   := -std=c2x
OPT   := -O2
DEBUG := -ggdb -fsanitize=address
INPUT := in.txt
LINK  := -lm -L$(OBJ) -Wl,--start-group $(LIB_NAMES) -Wl,--end-group

all: main

test: main $(INPUT)
	./main $(INPUT)

clean:
	$(RM) main obj/*

main: *.c $(LIB_ARCHIVES)
	$(CC) $(DEBUG) -o $@ *.c $(LINK) $(OPT)

$(OBJ)/lib%.a: $(OBJ)/%.o
	ar rcs $@ $<
	ranlib $@

$(OBJ)/%.o: $(LIB)/%.c $(LIB)/*.h
	$(CC) -c $< -o $@ $(OPT) $(DEBUG) $(WARN) $(STD)
