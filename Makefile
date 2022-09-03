WARN  = -Wpedantic -Werror -Wimplicit -Wall -Wno-unused-function
STD   = -std=c2x
OPT   = -O3
LINK  = -lm -L./obj -lstring -lparsing -lformat -lerror
DEBUG = -ggdb -fsanitize=address
INPUT = in.txt
SRC := lib
OBJ := obj
SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/lib%.o, $(SOURCES))
LIBS    := $(patsubst $(SRC)/%.c, $(OBJ)/lib%.a, $(SOURCES))

default: main

test: main $(INPUT)
	./main $(INPUT)

clean:
	$(RM) main main.o obj/*

main: *.c $(LIBS)
	$(CC) $(DEBUG) -o $@ *.c $(LINK) $(OPT)

$(OBJ)/%.a: $(OBJ)/%.o
	ar rcs $@ $<
	ranlib $@

$(OBJ)/lib%.o: $(SRC)/%.c $(SRC)/*.h
	$(CC) -I$(SRC): -c $< -o $@ $(OPT) $(DEBUG) $(WARN) $(STD)
