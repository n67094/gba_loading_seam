#include <tonc_input.h>
#include <tonc_oam.h>

#include "../core/core-global.h"
#include "../core/core-types.h"

#include "player.h"

#define PLAYER_SPEED 2

/*
const OBJ_ATTR playerObject[] = {
};
*/

void PlayerInit(Sprite* player, int x, int y, int objectId)
{
	player->position = (Point) { x, y };
	player->objectId = objectId;

	// 	oam_copy(&globalObjectBuffer[objectId], playerObject, );
}

void PlayerUpdate(Sprite* player)
{
	int velocityX = 0;
	int velocityY = 0;

	if (key_is_down(KEY_RIGHT)) {
		velocityX += PLAYER_SPEED;
	} else if (key_is_down(KEY_LEFT)) {
		velocityX -= PLAYER_SPEED;
	}

	if (key_is_down(KEY_DOWN)) {
		velocityY += PLAYER_SPEED;
	} else if (key_is_down(KEY_UP)) {
		velocityY -= PLAYER_SPEED;
	}

	player->position.x += velocityX;
	player->position.y += velocityY;
}

void PlayerDestroy(void)
{
}
