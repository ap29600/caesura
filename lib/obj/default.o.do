#!/bin/sh
redo-ifchange ../../options
. ../../options

SOURCE=../${2/\./\/}.c
redo-ifchange ../*.h ../**/*.h $SOURCE

$CC -c $SOURCE -o /dev/stdout $OPT $DEBUG $WARN $STD
