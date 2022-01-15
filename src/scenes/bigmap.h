#ifndef SCENE_BIGMAP
#define SCENE_BIGMAP

#include "../core/core-types.h"

typedef struct BigmapScene {
	TileMap layer0;
	TileMap layer1;
} BigmapScene;

extern BigmapScene bigmapScene;

void BigmapInit(void);

void BigmapUpdate(void);

void BigmapDestroy(void);

#endif
