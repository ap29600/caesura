#!/bin/sh

rm -f main obj/*.{o,a}
rm -r $(find . -type d -name '.redo')
