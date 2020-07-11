#!/bin/bash

for file in raw/*.wav; do
  ogg="$(basename $file)"
  ogg="${ogg%.*}.ogg"
  oggenc --downmix -o "$ogg" "$file"
  hexdump -v -e '1/1 "0x%x," ""' "$ogg" > "${ogg}.h"
  rm "$ogg"
done
