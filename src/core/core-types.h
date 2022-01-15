#ifndef CORE_TYPES
#define CORE_TYPES

#include <tonc_types.h>

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Viewport {
	Point position;
	Point size;
	Point start;
	Point end;
} Viewport;

typedef struct TileMap {
	int backgroundNumber;
	u32 backgroundControls;
	u32 mapWidth;
	u32 mapHeight;
	SCR_ENTRY* sourceMap;
	SCR_ENTRY* destinationMap;
	Point position;
} TileMap;

typedef struct Sprite {
	Point position;
	u8 objectId;
} Sprite;

#endif
