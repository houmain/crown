#!/usr/bin/bash

spright -a -i spright.conf -t cpp_header.template -o ../sprites.h -s ../sprites.png

hexdump -v -e '1/1 "0x%x," ""' ../sprites.png > ../sprites.png.h
