#!/bin/sh

rm main lib/obj/*.{o,a}

rm -r $(find . -type d -name '.redo')
