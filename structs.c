#include <structs.h>

const UINT8 GRAVITY = 2;
const UINT8 PLAYER_X_START = 80;
const UINT8 PLAYER_Y_START = 70;
const UINT8 PLAYER_SPRITES = 0;
const UINT8 PROJECTILE_SPRITES = 4;
const UINT8 CROUCHING_SPRITE = 6;
const UINT8 GLOBAL_FLOOR = 136;
const UINT8 PROJECTILE_SPEED = 5;

void performantdelay(UINT8 numloops)
{
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}
