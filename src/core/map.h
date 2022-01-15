#ifndef CORE_MAP
#define CORE_MAP

#include "core-types.h"

void MapInit(TileMap* tilemap, const void* map, u32 mapWidth, u32 mapHeight, const void* tiles, u32 tilesLength, int backgroundNumber, u32 backgroundControls, Viewport* viewport);

void MapUpdate(TileMap* tileMap, Viewport* viewport);

void MapDestroy(TileMap* tilemap);

#endif
