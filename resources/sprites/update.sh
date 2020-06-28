#!/usr/bin/bash

spright -a -i sprites.conf -t sprites.template -o ../sprites.h -s ../sprites.png
hexdump -v -e '1/1 "0x%x," ""' ../sprites.png > ../sprites.png.h

spright -a -i tiles.conf -t tiles.template -o ../tiles.h -s ../tiles.png
hexdump -v -e '1/1 "0x%x," ""' ../tiles.png > ../tiles.png.h
