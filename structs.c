#include <structs.h>

const UINT8 GRAVITY = 2;
const UINT8 PLAYER_X_START = 80;
const UINT8 PLAYER_Y_START = 70;
const UINT8 PLAYER_SPRITES = 0;
const UINT8 PROJECTILE_SPRITES = 4;
const UINT8 CROUCHING_SPRITE = 6;
const UINT8 GLOBAL_FLOOR = 136;
const UINT8 PROJECTILE_SPEED = 5;
const UINT8 JUMP_SPEED = -12;

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

void playFireSound()
{
    NR10_REG = 0x34U;
    NR11_REG = 0x70U;
    NR12_REG = 0xF0U;
    NR13_REG = 0x0AU;
    NR14_REG = 0xC6U;
    NR51_REG = 0x11;
}
