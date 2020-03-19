#include <gb/gb.h>
#include <stdio.h>
#include <Sprites.c>
#include <bgtiles.c>
#include <bg.c>
#include <structs.h>
#include <structs.c>
#include <GhostsSplash_data.c>
#include <GhostsSplash_map.c>


void main()
{
  Entity player = {
	  PLAYER_X_START,	// xLoc
	  PLAYER_Y_START,	// yLoc
	  0,				// xSpeed
	  0,				// ySpeed
	  3,				// numberSprites
	  False,			// isMoving
	  False,			// isCrouched
	  True,				// facingRight
	  True				// inAir
  };
  Projectile projectile = {
	  player.xLoc,		// xLoc
	  player.yLoc,		// yLoc
	  player.facingRight,//facingRight
	  4					// numberSprites
  };

  // used for swapping sprites.
  UINT8 currentSpriteIndex = 0;


  // projected y. used for collision detection.
  UINT8 projy = player.yLoc;



  // projectile y
  UINT8 py = 100;

  // use to help with delay for sprite changing
  UINT8 spritecount1 = 0;

  set_bkg_data(0, 500, GhostsSplash_data);
  set_bkg_tiles(0, 0, 20, 18, GhostsSplash_map);

  SHOW_BKG;
  DISPLAY_ON;

  waitpad(J_START);

  // sets background tile data from bgtiles label.
  set_bkg_data(0, 19, bgtiles);

  // sets background map data from backgroundmap label.
  set_bkg_tiles(0, 0, 80, 26, backgroundmap);

  // SHOW_BKG;
  // DISPLAY_ON;

  // loads sprite data, sets tiles for sprite 0 and 1.
  // puts sprites 0 and 1 on screen.
  set_sprite_data(0,7,Sprites);
  set_sprite_tile(0,PLAYER_SPRITES);
  set_sprite_tile(1,PROJECTILE_SPRITES);
  move_sprite(0,player.xLoc, player.yLoc);
  move_sprite(1,projectile.xLoc,projectile.yLoc);
  SHOW_SPRITES;



  while(1)
  {

    // joypad input functions
    if (joypad() & J_LEFT)
    {
      if (player.xLoc > 50)
      {
        scroll_sprite(0,-5,0);
        player.xLoc -= 5;
      }
      else
      {
        scroll_bkg(-5,0);
      }
      player.isMoving = True;
      set_sprite_tile(0, player.numberSprites - currentSpriteIndex);
      if (player.facingRight)
      {
        player.facingRight = False;
        set_sprite_prop(0,S_FLIPX);
      }

    }
    if (joypad() & J_RIGHT)
    {
      if (player.xLoc < 80) {
        scroll_sprite(0,5,0);
        player.xLoc += 5;
      }
      else
      {
        scroll_bkg(5,0);
      }
      player.isMoving = True;
      set_sprite_tile(0, player.numberSprites - currentSpriteIndex);

      if (!player.facingRight)
      {
        player.facingRight = True;
        set_sprite_prop(0,get_sprite_prop(0) & ~S_FLIPX);
      }
    }
    if (joypad() & J_UP)
    {
      scroll_sprite(0,0,-3);
      player.yLoc -= 3;
    }
    if (joypad() & J_DOWN)
    {
      player.isCrouched = True;
      set_sprite_tile(0, CROUCHING_SPRITE);
      scroll_sprite(0,0,3);
      player.yLoc += 3;
    }

	//jump
    if (joypad() & J_B)
    {
      if (!player.inAir)
      {
        player.inAir = True;
        player.ySpeed = -12;
        projy = player.yLoc + player.ySpeed;
      }
    }

	//projectile
	if (joypad() & J_A)
	{
	  move_sprite(1, player.xLoc, player.yLoc);
	  projectile.xLoc = player.xLoc;
	  projectile.yLoc = player.yLoc;

	  projectile.facingRight = player.facingRight;
	}

    // calculate projected y.
    projy = player.yLoc + player.ySpeed;

    // if next loop isn't under floor, keep falling.
    if (projy < GLOBAL_FLOOR)
    {
      player.yLoc = projy;
      scroll_sprite(0,0,player.ySpeed);
      player.ySpeed += GRAVITY;
    }

    else
    {
      player.yLoc = GLOBAL_FLOOR;
	  player.ySpeed = 0;
      move_sprite(0,player.xLoc, player.yLoc);
      player.inAir = False;
    }

    // if player isn't moving right, set to neutral tile.
    if (!player.isMoving && !player.isCrouched)
    {
      set_sprite_tile(0, currentSpriteIndex);
    }

    // if projectile is on screen, keep scrolling.
    if ((projectile.facingRight && projectile.xLoc < 200) || (!projectile.facingRight && projectile.xLoc > 0))
    {
      set_sprite_tile(1, projectile.numberSprites + currentSpriteIndex);
      if (projectile.facingRight)
      {
        scroll_sprite(1, PROJECTILE_SPEED, 0);
        projectile.xLoc += 5;
      }
      else
      {
        scroll_sprite(1 ,-1 * PROJECTILE_SPEED, 0);
        projectile.xLoc -= 5;
      }

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
    player.isMoving = False;
    player.isCrouched = False;
    performantdelay(4);
    spritecount1 +=1;
  }
}
