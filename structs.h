#ifndef STRUCTS_H
#define STRUCTS_H

extern const UINT8 GRAVITY;
extern const UINT8 PLAYER_X_START;
extern const UINT8 PLAYER_Y_START;
extern const UINT8 PLAYER_SPRITES;
extern const UINT8 PROJECTILE_SPRITES;
extern const UINT8 CROUCHING_SPRITE;
extern const UINT8 GLOBAL_FLOOR;
extern const UINT8 PROJECTILE_SPEED;

typedef enum
{
	False,
	True
} Boolean;

typedef struct
{
	UINT8 xLoc;
	UINT8 yLoc;
	UINT8 xSpeed;
	UINT8 ySpeed;
	UINT8 numberSprites;
	Boolean isMoving;
	Boolean isCrouched;
	Boolean facingRight;
	Boolean inAir;
} Entity;

typedef struct
{
	UINT8 xLoc;
	UINT8 yLoc;
	Boolean facingRight;
	UINT8 numberSprites;
} Projectile;

void performantdelay(UINT8 numloops);

#endif
