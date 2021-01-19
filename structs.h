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
extern const UINT8 JUMP_SPEED;
extern const UINT8 SOUL_DROP_SPEED;
extern const UINT8 NUM_SOULS;
extern const UINT8 PLAYER_WIDTH;
extern const UINT8 PLAYER_HEIGHT;
extern const UINT8 SOUL_WIDTH;
extern const UINT8 SOUL_HEIGHT;

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

typedef struct Soul
{
	UINT8 xLoc;
	UINT8 yLoc;
} Soul;


void performantdelay(UINT8 numloops);

void playJumpSound();

void playFireSound();

Boolean hasCollision(Soul *soul, Entity *ent);

#endif
