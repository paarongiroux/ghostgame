#include <gb/gb.h>
#include <stdio.h>
#include <Sprites.c>
#include <bgtiles.c>
#include <bg.c>


void performantdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}

void main()
{
  // used for swapping sprites.
  UINT8 currentSpriteIndex = 0;

  // boolean. 1 if player is in the air.
  UINT8 inAir = 0;

  // holds player's yspeed. used for jumping/falling.
  UINT8 yspeed = 0;

  // gravity constant.
  UINT8 grav = 2;

  // floor y position constant.
  UINT8 floor = 120;

  // player x
  UINT8 x = 80;

  // player y
  UINT8 y = 70;

  // projected y. used for collision detection.
  UINT8 projy = y;

  // projectile x
  UINT8 px = 100;

  // projectile y
  UINT8 py = 100;

  // boolean. 1 if player is moving right.
  UINT8 moving = 0;

  UINT8 crouched = 0;

  UINT8 facingRight = 1;

  int spritecount1 = 0;
  int spritecount2 = 0;


  // sets background tile data from bgtiles label.
  set_bkg_data(0, 7, bgtiles);

  // sets background map data from backgroundmap label.
  set_bkg_tiles(0, 0, 40, 18, backgroundmap);

  SHOW_BKG;
  DISPLAY_ON;

  // loads sprite data, sets tiles for sprite 0 and 1.
  // puts sprites 0 and 1 on screen.
  set_sprite_data(0,7,Sprites);
  set_sprite_tile(0,0);
  set_sprite_tile(1,4);
  move_sprite(0,x,y);
  move_sprite(1,px,py);
  SHOW_SPRITES;



  while(1)
  {

    // joypad input functions
    if (joypad() & J_LEFT)
    {
      if (x > 50)
      {
        scroll_sprite(0,-5,0);
        x -= 5;
      }
      else
      {
        scroll_bkg(-5,0);
      }
      moving = 1;
      set_sprite_tile(0,3 - currentSpriteIndex);
      if (facingRight)
      {
        facingRight = 0;
        set_sprite_prop(0,S_FLIPX);
      }

    }
    if (joypad() & J_RIGHT)
    {
      if (x < 80) {
        scroll_sprite(0,5,0);
        x += 5;
      }
      else
      {
        scroll_bkg(5,0);
      }
      moving = 1;
      set_sprite_tile(0,3 - currentSpriteIndex);

      if (!facingRight)
      {
        facingRight = 1;
        set_sprite_prop(0,get_sprite_prop(0) & ~S_FLIPX);
      }
    }
    if (joypad() & J_UP)
    {
      scroll_sprite(0,0,-3);
      y -= 3;
    }
    if (joypad() & J_DOWN)
    {
      crouched = 1;
      set_sprite_tile(0,6);
      scroll_sprite(0,0,3);
      y += 3;
    }
    if (joypad() & J_A)
    {
      move_sprite(1, x, y);
      px = x;
      py = y;
    }
    if (joypad() & J_B)
    {
      if (inAir == 0)
      {
        inAir = 1;
        yspeed = -12;
        projy = y + yspeed;
      }
    }

    // calculate projected y.
    projy = y + yspeed;

    // if next loop isn't under floor, keep falling.
    if (projy < floor)
    {
      y = projy;
      scroll_sprite(0,0,yspeed);
      yspeed += grav;
    }
    // if next loop iter goes thru floor, snap to floor now.
    if (projy >= floor)
    {
      y = floor;
      projy = y;
      yspeed = 0;
      move_sprite(0,x,y);
      inAir = 0;
    }

    // if player isn't moving right, set to neutral tile.
    if (!moving && !crouched)
    {
      set_sprite_tile(0, currentSpriteIndex);
    }

    // if projectile is on screen, keep scrolling.
    if (px < 200)
    {
      scroll_sprite(1,5,0);
      set_sprite_tile(1, 4 + currentSpriteIndex);
      px += 5;
    }

    if (spritecount1 > 4)
    {
      spritecount1 = 0;
      if (currentSpriteIndex)
      {
        currentSpriteIndex = 0;
      }
      else
      {
        currentSpriteIndex = 1;
      }
    }

    // set moveright to false before each iter
    moving = 0;
    crouched = 0;
    performantdelay(4);
    spritecount1 +=1;
  }
}
