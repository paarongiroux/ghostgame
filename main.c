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
  // stores all data associated with the player
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

  // stores all data associated with the player's projectile
  Projectile projectile = {
	  player.xLoc,		// xLoc
	  player.yLoc,		// yLoc
	  player.facingRight,//facingRight
	  4					// numberSprites
  };

  // used for swapping sprites.
  UINT8 currentSpriteIndex = 0;


  // player's projected y.
  // stores player's anticipated location for the next frame.
  // used for collision detection.
  UINT8 projy = player.yLoc;


  // counter used to help with delay for sprite changing.
  UINT8 spritecount1 = 0;

  // sets initial background splash screen.
  set_bkg_data(0, 189, GhostsSplash_data);
  set_bkg_tiles(1, 0, 18, 18, GhostsSplash_map);

  // show background, set display to on.
  SHOW_BKG;
  DISPLAY_ON;

  // wait for player to press start before starting the game.
  waitpad(J_START);

  // sets background tile data from bgtiles label.
  set_bkg_data(0, 19, bgtiles);

  // sets background map data from backgroundmap label.
  set_bkg_tiles(0, 0, 80, 26, backgroundmap);

  // loads sprite data, sets tiles for sprite 0 and 1.
  // puts sprites 0 and 1 on screen.
  set_sprite_data(0,7,Sprites);

  // set sprite 0 to PLAYER_SPRITES index.
  set_sprite_tile(0,PLAYER_SPRITES);
  // set sprite 1 to PROJECTILE_SPRITES index.
  set_sprite_tile(1,PROJECTILE_SPRITES);

  // moe sprites to their starting locations, show sprites.
  move_sprite(0,player.xLoc, player.yLoc);
  move_sprite(1,projectile.xLoc,projectile.yLoc);
  SHOW_SPRITES;


  // MAIN GAME LOOP ===========================================================
  while(1)
  {
	// check for joypad input ============================
	// case: Left D-Pad ================
    if (joypad() & J_LEFT)
    {
	  // if the player is on the far right of the screen, scroll the player.
      if (player.xLoc > 50)
      {
        scroll_sprite(0,-5,0);
        player.xLoc -= 5;
      }
	  // otherwise scroll the background.
      else
      {
        scroll_bkg(-5,0);
      }
	  // set movement flag, handle sprite animation.
      player.isMoving = True;
      set_sprite_tile(0, player.numberSprites - currentSpriteIndex);

	  // if player was facing right, flip sprite so he's facing left.
      if (player.facingRight)
      {
        player.facingRight = False;
        set_sprite_prop(0,S_FLIPX);
      }
    }

	// case: Right D-Pad ================
    if (joypad() & J_RIGHT)
    {
	  // if player is on far left of the screen, scroll the player.
      if (player.xLoc < 80)
	  {
        scroll_sprite(0,5,0);
        player.xLoc += 5;
      }
	  // otherwise, scroll the background
      else
      {
        scroll_bkg(5,0);
      }
	  // set movement flag, handle sprite animation.
      player.isMoving = True;
      set_sprite_tile(0, player.numberSprites - currentSpriteIndex);

	  // if player was facing left, flip sprite so he's facing left.
      if (!player.facingRight)
      {
        player.facingRight = True;
        set_sprite_prop(0,get_sprite_prop(0) & ~S_FLIPX);
      }
    }

	// case: Up D-Pad ================
    if (joypad() & J_UP)
    {
	  // if player is not in the air, pressing up will give a small bounce
	  if (!player.inAir)
	  {
      	scroll_sprite(0,0,-3);
      	player.yLoc -= 3;
	  }
    }
	// case: Down D-Pad ================
    if (joypad() & J_DOWN)
    {
	  // set crouched flag, set sprite to CROUCHING_SPRITE
      player.isCrouched = True;
      set_sprite_tile(0, CROUCHING_SPRITE);

	  // scroll player down (fastfalling mechanic)
      scroll_sprite(0,0,3);
      player.yLoc += 3;
    }

	// case: B button (jump) ================
    if (joypad() & J_B)
    {
	  // make sure player is not in air (no double jumps)
      if (!player.inAir)
      {
		// set inAir flag and initial jump speed
        player.inAir = True;
        player.ySpeed = JUMP_SPEED;

		// calculate player's projected yLocation
        projy = player.yLoc + player.ySpeed;
      }
    }

	// case: A button (projectile) ====================
	if (joypad() & J_A)
	{
	  // move projectile sprite to the players location & direction.
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

	// otherwise, next frame will put the player below the floor.
	// snap to floor instead.
    else
    {
      player.yLoc = GLOBAL_FLOOR;
	  player.ySpeed = 0;
      move_sprite(0,player.xLoc, player.yLoc);
      player.inAir = False;
    }

    // if player isn't moving, set to neutral tile.
    if (!player.isMoving && !player.isCrouched)
    {
      set_sprite_tile(0, currentSpriteIndex);
    }

    // if projectile is on screen, keep scrolling.
    if ((projectile.facingRight && projectile.xLoc < 200) || (!projectile.facingRight && projectile.xLoc > 0))
    {
	  // handle sprite animation
      set_sprite_tile(1, projectile.numberSprites + currentSpriteIndex);
	  // if projectile is facing right, keep scrolling to the right
      if (projectile.facingRight)
      {
        scroll_sprite(1, PROJECTILE_SPEED, 0);
        projectile.xLoc += PROJECTILE_SPEED;
      }
	  // otherwise, keep scrolling to the left
      else
      {
        scroll_sprite(1 ,-1 * PROJECTILE_SPEED, 0);
        projectile.xLoc -= PROJECTILE_SPEED;
      }
    }

	// handle spritecount1. If greater than four: reset & flip spriteIndex.
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

    // set moving and crouched flags to false before each iteration.
    player.isMoving = False;
    player.isCrouched = False;
    performantdelay(4);
    spritecount1 +=1;
  }
}
