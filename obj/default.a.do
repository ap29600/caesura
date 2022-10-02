#!/bin/sh
redo-ifchange ../options
. ../options

RELEVANT_OBJECTS=$( find ../lib/${2##lib} -type f -name '*.c' |\
                    sed -e 's/\.\.\/lib\///' -e 's/\//\./g' -e's/\.c$/\.o/' )

redo-ifchange $RELEVANT_OBJECTS

ar rcs $3.a $RELEVANT_OBJECTS 1>&2
ranlib $3.a 1>&2
mv $3.a $3
