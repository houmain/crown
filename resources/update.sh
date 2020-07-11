#!/bin/bash

resource() {
  id=$1
  path=$2
  if [ "$path" -nt "_generated/${id}.inc" ]; then
    hexdump -v -e '1/1 "0x%x," ""' "$path" > "_generated/${id}.inc"
  fi

cat <<EOF >> "_generated/resources.inc"
RESOURCE_BEGIN("${id}")
#include "${id}.inc"
RESOURCE_END

EOF
}

mkdir -p _generated
echo "" > _generated/resources.inc

# sprites
spright -a -i sprites.conf -t sprites.template -o _generated/sprites.h -s _generated/sprites.png
resource sprites.png _generated/sprites.png

spright -a -i tiles.conf -t tiles.template -o _generated/tiles.h -s _generated/tiles.png
resource tiles.png _generated/tiles.png

# music
resource dualtrax.xm music/dualtrax.xm

#fonts
resource upheavtt.ttf fonts/upheavtt.ttf

# sounds
resource swing.ogg sounds/rpg-sound-pack/battle/swing.ogg
resource swing2.ogg sounds/rpg-sound-pack/battle/swing2.ogg
resource swing3.ogg sounds/rpg-sound-pack/battle/swing3.ogg
resource grunt1.ogg sounds/rpg-sound-pack/NPC/shade/shade1.ogg

