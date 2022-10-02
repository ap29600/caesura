#!/bin/sh
redo-ifchange ../options
. ../options

SOURCE=../lib/${2/\./\/}.c
redo-ifchange ../lib/*.h ../lib/**/*.h $SOURCE

$CC -c $SOURCE -o /dev/stdout $OPT $DEBUG $WARN $STD
