#include <structs.h>

const UINT8 GRAVITY = 2;
const UINT8 PLAYER_X_START = 80;
const UINT8 PLAYER_Y_START = 70;
const UINT8 PLAYER_SPRITES = 0;
const UINT8 PROJECTILE_SPRITES = 4;
const UINT8 SOUL_SPRITES = 7;
const UINT8 CROUCHING_SPRITE = 6;
const UINT8 GLOBAL_FLOOR = 136;
const UINT8 PROJECTILE_SPEED = 5;
const UINT8 JUMP_SPEED = -9;
const UINT8 SOUL_DROP_SPEED = 2;
const UINT8 PLAYER_WIDTH = 8;
const UINT8 PLAYER_HEIGHT = 8;
const UINT8 SOUL_WIDTH = 8;
const UINT8 SOUL_HEIGHT = 8;
const UINT8 PLAYER_SPEED = 7;

void performantdelay(UINT8 numloops)
{
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}

void playJumpSound()
{
    NR10_REG = 0x16; 
    NR11_REG = 0x40;
    NR12_REG = 0x73;  
    NR13_REG = 0x00;   
    NR14_REG = 0xC3;
}


void playCollectSound()
{
    NR10_REG = 0x34U;
    NR11_REG = 0x80U;
    NR12_REG = 0xF0U;
    NR13_REG = 0x0AU;
    NR14_REG = 0xC6U;
    NR51_REG = 0x11;
}

void playDeathSound(){
	//Sound : Game Over sound
	NR10_REG = 0x7D; //or 1E or 1D for louder sound / 2E / 3E / 4E... for more "vibe"
	NR11_REG = 0xBF;
	NR12_REG = 0xF6; //B7, C7, D7...F7 for longer sound
	NR13_REG = 0x7C;
	NR14_REG = 0x86;
}

Boolean hasCollision(Soul *soul, Entity *ent)
{
    if ((soul->xLoc + SOUL_WIDTH >= ent->xLoc &&
        soul->xLoc + SOUL_WIDTH <= ent->xLoc + PLAYER_WIDTH) &&
        (soul->yLoc + SOUL_HEIGHT >= ent->yLoc &&
        soul->yLoc + SOUL_HEIGHT <= ent->yLoc + PLAYER_HEIGHT)) 
    {
        return True;
    }
    if ((ent->xLoc + PLAYER_WIDTH >= soul->xLoc &&
        ent->xLoc + PLAYER_WIDTH <= soul->xLoc + SOUL_WIDTH) &&
        (ent->yLoc + PLAYER_HEIGHT >= soul->yLoc &&
        ent->yLoc + PLAYER_HEIGHT <= soul->yLoc + SOUL_HEIGHT)) 
    {
        return True;
    }
    return False;
}
