#!/bin/sh

sed -i  -e 's/[ \t]\+$//'  \
	src/main.c src/**/*.{c,h}
