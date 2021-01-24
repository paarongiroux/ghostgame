#include <gb/gb.h>
#include <stdio.h>
#include <Sprites.c>
#include <bgtiles.c>
#include <bg.c>
#include <structs.h>
#include <structs.c>
#include <GhostsSplash_data.c>
#include <GhostsSplash_map.c>
#include <insides_data.c>
#include <insides_map.c>
#include <stdio.h>
#include <stdlib.h>
#include <rand.h>

void runGame();

void main()
{
  // draw intro screen
  // sets initial background splash screen.
  set_bkg_data(0, 40, insides_data);
  set_bkg_tiles(0, 0, 20, 18, insides_map);

  // show background, set display to on.
  SHOW_BKG;
  DISPLAY_ON;
  delay(2500);
  HIDE_BKG;
  DISPLAY_OFF;
  while (True) {
    
    runGame();
  }
  
}

void runGame() {
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

  // // stores all data associated with the player's projectile
  // Projectile projectile = {
	//   player.xLoc,		// xLoc
	//   player.yLoc,		// yLoc
	//   player.facingRight,//facingRight
	//   4					// numberSprites
  // };

  // possible xLocations for the souls to spawn
  UINT8 soulSpawns[22] = {
    8  , 15 , 22 , 29 , 36 , 43 , 50,
    57 , 64 , 71 , 78 , 85 , 92 , 99,
    106, 113, 120, 127, 134, 141, 148, 155};

  // Souls data
  Soul soul1 = {
    16,
    10
  };
  Soul soul2 = {
    48,
    10
  };
  Soul soul3 = {
    80,
    10
  };
  Soul soul4 = {
    112,
    10
  };
  Soul soul5 = {
    144,
    10
  };

  // tracks for gameOver
  Boolean gameOver = False;

  // used for swapping sprites.
  UINT8 currentSpriteIndex = 0;

  // player's projected y.
  // stores player's anticipated location for the next frame.
  // used for collision detection.
  UINT8 projy = player.yLoc;

  // counter used to help with delay for sprite changing.
  UINT8 spritecount1 = 0;

  // counter used to time souls initial dropping.
  UINT8 soulInitCounter = 0;

  // tracks the number of souls collected by the player.
  UINT8 soulsCollected = 0;

  HIDE_BKG;
  DISPLAY_OFF;
  // sets initial background splash screen.
  set_bkg_data(0, 100, GhostsSplash_data);
  set_bkg_tiles(0, 0, 20, 18, GhostsSplash_map);

  // show background, set display to on.
  SHOW_BKG;
  DISPLAY_ON;

  // wait for player to press start before starting the game.
  waitpad(J_START);

  // sets background tile data from bgtiles label.
  set_bkg_data(0, 19, bgtiles);

  // sets background map data from backgroundmap label.
  set_bkg_tiles(0, 0, 20, 18, backgroundmap);

  // loads sprite data, sets tiles for sprite 0 and 1.
  // puts sprites 0 and 1 on screen.
  set_sprite_data(0,8,Sprites);

  // set sprite 0 to PLAYER_SPRITES index.
  set_sprite_tile(0,PLAYER_SPRITES);
  // // set sprite 1 to PROJECTILE_SPRITES index.
  // set_sprite_tile(1,PROJECTILE_SPRITES);

  // SPRITES 2-6 are soul sprites
  // set sprite 2 to PLAYER_SPRITES index.
  set_sprite_tile(2,SOUL_SPRITES);
  // set sprite 3 to PLAYER_SPRITES index.
  set_sprite_tile(3,SOUL_SPRITES);
  // set sprite 4 to PLAYER_SPRITES index.
  set_sprite_tile(4,SOUL_SPRITES);
  // set sprite 5 to PLAYER_SPRITES index.
  set_sprite_tile(5,SOUL_SPRITES);
  // set sprite 6 to PLAYER_SPRITES index.
  set_sprite_tile(6,SOUL_SPRITES);
  

  // moe sprites to their starting locations, show sprites.
  move_sprite(0,player.xLoc, player.yLoc);
  // move_sprite(1,projectile.xLoc, projectile.yLoc);

  // move soul sprites to their starting locations.
  move_sprite(2, soul1.xLoc, soul1.yLoc);
  move_sprite(3, soul2.xLoc, soul2.yLoc);
  move_sprite(4, soul3.xLoc, soul3.yLoc);
  move_sprite(5, soul4.xLoc, soul4.yLoc);
  move_sprite(6, soul5.xLoc, soul5.yLoc);

  SHOW_SPRITES;


  NR52_REG = 0x80;
  NR50_REG = 0x77;
  NR51_REG = 0xFF;


  // for (soulIndex = 0; soulIndex < NUM_SOULS; soulIndex++)
  // {
  //   int x = soulIndex;
  // }


  // MAIN GAME LOOP ===========================================================
  while(1)
  {
	// check for joypad input ============================
	// case: Left D-Pad ================
    if (joypad() & J_LEFT)
    {
      scroll_sprite(0,(-1 * PLAYER_SPEED),0);
      player.xLoc -= PLAYER_SPEED;
      if ((int) player.xLoc <= 2 && (int) player.xLoc > -90) 
      {
        player.xLoc = -90;
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
      scroll_sprite(0,PLAYER_SPEED,0);
      player.xLoc += PLAYER_SPEED;

      // ran off left edge of screen.
      if (player.xLoc >= 168)
      {
        player.xLoc = 8;
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
        playJumpSound();
		// set inAir flag and initial jump speed
        player.inAir = True;
        player.ySpeed = JUMP_SPEED;

		// calculate player's projected yLocation
        projy = player.yLoc + player.ySpeed;
      }
    }

	// // case: A button (projectile) ====================
	// if (joypad() & J_A)
	// {
  //   playFireSound();
	//   // move projectile sprite to the players location & direction.
	//   move_sprite(1, player.xLoc, player.yLoc);
	//   projectile.xLoc = player.xLoc;
	//   projectile.yLoc = player.yLoc;
	//   projectile.facingRight = player.facingRight;
	// }

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

    // // if projectile is on screen, keep scrolling.
    // if ((projectile.facingRight && projectile.xLoc < 200) || (!projectile.facingRight && projectile.xLoc > 0))
    // {
	  // // handle sprite animation
    //   set_sprite_tile(1, projectile.numberSprites + currentSpriteIndex);
	  // // if projectile is facing right, keep scrolling to the right
    //   if (projectile.facingRight)
    //   {
    //     scroll_sprite(1, PROJECTILE_SPEED, 0);
    //     projectile.xLoc += PROJECTILE_SPEED;
    //   }
	  // // otherwise, keep scrolling to the left
    //   else
    //   {
    //     scroll_sprite(1 ,-1 * PROJECTILE_SPEED, 0);
    //     projectile.xLoc -= PROJECTILE_SPEED;
    //   }
    // }

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
    
    // check collision here
    if (hasCollision(&soul1, &player)) 
    {
      soul1.xLoc = soulSpawns[(rand() & 21)]; 
      soul1.yLoc = 10;
      playCollectSound();
    }
    if (hasCollision(&soul2, &player)) 
    {
      soul2.xLoc = soulSpawns[(rand() & 21)]; 
      soul2.yLoc = 10;
      playCollectSound();
    }
    if (hasCollision(&soul3, &player)) 
    {
      soul3.xLoc = soulSpawns[(rand() & 21)]; 
      soul3.yLoc = 10;
      playCollectSound();
    }
    if (hasCollision(&soul4, &player)) 
    {
      soul4.xLoc = soulSpawns[(rand() & 21)]; 
      soul4.yLoc = 10;
      playCollectSound();
    }
    if (hasCollision(&soul5, &player)) 
    {
      soul5.xLoc = soulSpawns[(rand() & 21)]; 
      soul5.yLoc = 10;
      playCollectSound();
    }

    // handle soul1 movement
    if (soul1.yLoc > 160) 
    {
      gameOver = True;
    }
    else
    {
      soul1.yLoc += SOUL_DROP_SPEED;
      move_sprite(2,soul1.xLoc, soul1.yLoc);
    } 
    // handle soul2 movement
    if (soul2.yLoc > 160)
    {
      gameOver = True;
    }
    else
    {
      if (soulInitCounter > 15) 
      {
        soul2.yLoc += SOUL_DROP_SPEED;
        move_sprite(3,soul2.xLoc, soul2.yLoc);
      }
    }
    // handle soul3 movement
    if (soul3.yLoc > 160)
    {
      gameOver = True;
    }
    else 
    {
      if (soulInitCounter > 30) 
      {
        soul3.yLoc += SOUL_DROP_SPEED;
        move_sprite(4,soul3.xLoc, soul3.yLoc);
      }
    }
    // handle soul4 movement
    if (soul4.yLoc > 160)
    {
      gameOver = True;
    }
    else 
    {
      if (soulInitCounter > 45) 
      {
        soul4.yLoc += SOUL_DROP_SPEED;
        move_sprite(5,soul4.xLoc, soul4.yLoc);
      } 
    }
    // handle soul5 movement
    if (soul5.yLoc > 160)
    {
      gameOver = True;
    }
    else 
    {
      if (soulInitCounter > 60) 
      {
        soul5.yLoc += SOUL_DROP_SPEED;
        move_sprite(6,soul5.xLoc, soul5.yLoc);
      }
    }
    

    // set moving and crouched flags to false before each iteration.
    player.isMoving = False;
    player.isCrouched = False;
    performantdelay(4);
    spritecount1 +=1;

    if (soulInitCounter <= 60) {
      soulInitCounter++;
    }

    if (gameOver) {
      break;
    }
  }
  HIDE_SPRITES;
  HIDE_BKG;
  DISPLAY_OFF;
  set_bkg_data(0, 100, GhostsSplash_data);
  set_bkg_tiles(0, 0, 20, 18, GhostsSplash_map);
  SHOW_BKG;
  DISPLAY_ON;
}
