#!/bin/bash

while read -rd $'\0' wav; do
  ogg="${wav%.*}.ogg"
  oggenc --downmix -o "$ogg" "$wav"
  rm "$wav"
done < <(find  . -type f -name '*.wav' -print0)
