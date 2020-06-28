
// to create headers files use:
// hexdump -v -e '1/1 "0x%x," ""' $1 > $1.h

RESOURCE_BEGIN("sprites.png")
#include "sprites.png.h"
RESOURCE_END

RESOURCE_BEGIN("tiles.png")
#include "tiles.png.h"
RESOURCE_END

RESOURCE_BEGIN("upheavtt.ttf")
#include "upheavtt.ttf.h"
RESOURCE_END

RESOURCE_BEGIN("dualtrax.xm")
#include "dualtrax.xm.h"
RESOURCE_END
