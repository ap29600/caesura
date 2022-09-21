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

define objects_of =
	$(shell sh -c "find lib/$(1) -type f -name '*.c' | sed 's/lib\/$(1)\//obj\/$(1)_/g; s/\.c/\.o/g'")
endef

define source_of =
	$(shell sh -c "echo $(1) | sed 's/_/\//; s/obj\//lib\//g; s/\.o/\.c/g'")
endef

all: main

test: main $(INPUT)
	./main $(INPUT)

clean:
	$(RM) main obj/*

main: $(SRC_MODULES) $(SRC_HEADERS) $(LIB_ARCHIVES)
	$(CC) $(DEBUG) -o $@ -iquote. $(SRC_MODULES) $(LINK) $(OPT)

define generate_archive =
 LIB_OBJECTS += $(call objects_of,$(1))
 $(OBJ)/lib$(1).a: $(call objects_of,$(1))
	ar rcs $$@ $$^
	ranlib $$@
endef

$(foreach arch,$(LIB_MODULES),$(eval $(call generate_archive,$(arch))))

define generate_object =
$(1): $(call source_of,$(1)) $$(LIB_HEADERS)
	$$(CC) -c $$< -o $$@ $$(OPT) $$(DEBUG) $$(WARN) $$(STD)
endef

$(foreach obj,$(LIB_OBJECTS),$(eval $(call generate_object,$(obj))))
