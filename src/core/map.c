#include <string.h>

#include <tonc_core.h>
#include <tonc_video.h>

#include "core-types.h"

#include "map.h"

// Fill a column of 32 Tiles on the left/right
static void MapAddColumn(TileMap* tileMap, int sourceTileX, int sourceTileY)
{
	// Find a corresponding column in the 32x32 tile map
	int destinationTileX = sourceTileX % 32;
	int destinationTileY = sourceTileY % 32;

	// SCR_ENTRY of the newly exposed tile, top left/right edge of the viewport, in sourceEntry
	SCR_ENTRY* sourceEntry = &tileMap->sourceMap[sourceTileY * tileMap->mapWidth + sourceTileX];

	// SCR_ENTRY, left/right edge of the viewport, to write new tiles
	SCR_ENTRY* destinationEntry = &tileMap->destinationMap[destinationTileY * 32 + destinationTileX];

	// Fill tiles one by one from destinationTileY to 31 (0 to 31 = 32th tile)
	for (int y = destinationTileY; y < 32; y++) {
		// Write a single tile
		*destinationEntry = *sourceEntry;

		// Move destinationEntry pointer down one tile (destinationTileY++)
		destinationEntry += 32;
		// Move sourceEntry pointer down one tile (sourceTileY++)
		sourceEntry += tileMap->mapWidth;
	}

	// Move destinationEntry pointer at position y = 0
	destinationEntry -= 1024;

	// Fill tiles one by one from 0 to destinationEntry - 1
	for (int y = 0; y < destinationTileY; y++) {
		*destinationEntry = *sourceEntry;

		destinationEntry += 32;
		sourceEntry += tileMap->mapWidth;
	}
}

// Fill a row of 32 Tiles on the top/bottom
static void MapAddRow(TileMap* tileMap, int sourceTileX, int sourceTileY)
{
	// Find a corresponding row in the 32x32 tile map
	int destinationTileX = sourceTileX % 32;
	int destinationTileY = sourceTileY % 32;

	// SCR_ENTRY of the newly exposed tile, top/bottom edge of the viewport, in sourceEntry
	SCR_ENTRY* sourceEntry = &tileMap->sourceMap[sourceTileY * tileMap->mapWidth + sourceTileX];

	// SCR_ENTRY, top/bottom edge of the viewport, to write the new tiles
	SCR_ENTRY* destinationEntry = &tileMap->destinationMap[destinationTileY * 32 + destinationTileX];

	// Fill tiles one by one from destinationTileX to 31 (0 to 31 = 32th tile)
	for (int x = destinationTileX; x < 32; x++) {
		*destinationEntry = *sourceEntry;

		destinationEntry++;
		sourceEntry++;
	}

	// Move destinationEntry pointer back to position x = 0
	destinationEntry -= 32;

	// Fill tiles one by one from 0 to destinationTileX - 1
	for (int x = 0; x < destinationTileX; x++) {
		*destinationEntry = *sourceEntry;

		destinationEntry++;
		sourceEntry++;
	}
}

// Init the map, (note that the palette should be initialized separately since they are only one palette of your choice for that background)
void MapInit(TileMap* tilemap, const void* map, u32 mapWidth, u32 mapHeight, const void* tiles, u32 tilesLength, int backgroundNumber, u32 backgroundControls, Viewport* viewport)
{
	// Allocat and Init TileMap with 0
	memset(tilemap, 0, sizeof(TileMap));

	// Init TileMap with data
	tilemap->backgroundNumber = backgroundNumber;
	tilemap->backgroundControls = backgroundControls;
	tilemap->sourceMap = (SCR_ENTRY*)map;
	tilemap->destinationMap = se_mem[BFN_GET(backgroundControls, BG_SBB)]; // Get the SBB flag set in backgroundControls
	tilemap->mapWidth = mapWidth;
	tilemap->mapHeight = mapHeight;
	tilemap->mapHeight = mapHeight;

	// Init background registers
	REG_BGCNT[backgroundNumber] = backgroundControls;

	// Init scroll at top left corner of thew viewport
	REG_BG_OFS[backgroundNumber].x = viewport->position.x;
	REG_BG_OFS[backgroundNumber].y = viewport->position.y;

	// Copy base tiles to charblock x where x = CBB flag set in backgroundControls
	memcpy32(tile_mem[BFN_GET(backgroundControls, BG_CBB)], tiles, tilesLength);

	// Tile coordinates of the top left corner of the viewport (>> 3 divide by 8)
	int tileX = viewport->position.y >> 3;
	int tileY = viewport->position.x >> 3;

	// Copy first 32x32 tiles around the start pos. from sourceEntry into destinationMap (se_mem)
	for (int y = 0; y < 32; y++) {
		MapAddRow(tilemap,
				tileX, tileY);
		tileY++;
	}
}

// Check if the viewport has shifted to another tile (up/down/left/right) this frame.
// If yes, copy 32 tiles row/column.
//
// The result is that the viewport will always show a 240x160p slice of destinationEntry that corresponds
// to the "camera's position" in sourceEntry (centred around the player's position).
void MapUpdate(TileMap* tileMap, Viewport* viewport)
{
	// Pixels coords
	int viewportX = viewport->position.x;
	int viewportY = viewport->position.y;

	int backgroundX = tileMap->position.x;
	int backgroundY = tileMap->position.y;

	// Tiles coords (>> 3 divide by 8)
	int tileViewportX = viewportX >> 3;
	int tileViewportY = viewportY >> 3;

	int tileBackgroundX = backgroundX >> 3;
	int tileBackgroundY = backgroundY >> 3;

	if (tileViewportX < tileBackgroundX) { // Add tiles to the left
		MapAddColumn(tileMap, tileViewportX, tileViewportY);
	} else if (tileViewportX > tileBackgroundX) { // Add tiles to the right
		MapAddColumn(tileMap, tileViewportX + 31, tileViewportY);
	}

	if (tileViewportY < tileBackgroundY) { // Add tiles to the top
		MapAddRow(tileMap, tileViewportX, tileViewportY);
	} else if (tileViewportY > tileBackgroundY) { // Add tiles to the bottom
		MapAddRow(tileMap, tileViewportX, tileViewportY + 31);
	}

	// Update TileMap position
	tileMap->position = viewport->position;

	// Scroll the background via register
	REG_BG_OFS[tileMap->backgroundNumber].x = (u16)viewport->position.x;
	REG_BG_OFS[tileMap->backgroundNumber].y = (u16)viewport->position.y;
}

// Reset a TileMap object with 0
void MapDestroy(TileMap* tilemap)
{
	memset(tilemap, 0, sizeof(TileMap));
}
