#ifndef ENTITIES_PLAYER
#define ENTITIES_PLAYER

#include "../core/core-types.h"

void PlayerInit(Sprite* player, int x, int y, int objectId);

void PlayerUpdate(Sprite* player);

void PlayerDestroy(void);

#endif
