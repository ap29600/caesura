#!/bin/sh
redo-ifchange options
. options

LIB_ARCHIVES=$( find lib/ -maxdepth 2 -type f -name '*.c'        |\
                sed 's/lib\/\([^/]*\)\/.*\.c/lib\/obj\/lib\1.a/' |\
                uniq )

LIB_NAMES=$( echo $LIB_ARCHIVES |\
             tr ' ' '\n'        |\
             sed 's/lib\/obj\/lib\([^/]*\).a/-l\1/' )

redo-ifchange $LIB_ARCHIVES
redo-ifchange src/main.c src/**/*.{c,h}

LINK="-lm -Llib/obj -Wl,--start-group $LIB_NAMES -Wl,--end-group"

$CC -o /dev/stdout src/*.c src/**/*.c $STD $OPT $DEBUG $WARN $LINK -iquote.

