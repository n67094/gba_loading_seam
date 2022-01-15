#include <tonc_bios.h>
#include <tonc_core.h>
#include <tonc_input.h>
#include <tonc_oam.h>
#include <tonc_video.h>

#include "../core/core-global.h"
#include "../core/core-types.h"
#include "../core/map.h"
#include "../core/viewport.h"

#include "../entities/player.h"

#include "bigmap.h"

// Background data
#include "BG0_map.h"
#include "BG0_palette.h"
#include "BG0_tiles.h"

// Background data
#include "BG1_map.h"
#include "BG1_palette.h"
#include "BG1_tiles.h"

BigmapScene bigmapScene;

void BigmapInit()
{
	globalViewport = (Viewport) {
		{ 512, 512 },										 // Start position, top left corner of the viewport
		{ SCREEN_WIDTH, SCREEN_HEIGHT }, // GBA screen info
		{ 0, 0 },												 // Bounding box top left corner
		{ 1024, 1024 }									 // Bounding box bottom left corner
	};

	// TODO add a sprite for the player

	// Init player in the center of the viewport (aka screen)
	PlayerInit(&globalplayer, 512 + (SCREEN_WIDTH >> 1), 512 + (SCREEN_HEIGHT >> 1), 0);

	memcpy16(pal_bg_bank[0], BG0_palette, 16); // 16 is the number of halfword (16bits) to fill, 1 color is 16 bit
	MapInit(
			&bigmapScene.layer0,
			BG0_map,
			128,
			128,
			BG0_tiles,
			BG0_tiles_size,
			0,
			// Priority 3, Tiles data -> on the 1th Charblock, TileMap data -> on the 31th screen entry (32x32 TileMap of 16 bit = 2048bytes = 1 Screen entry)
			BG_PRIO(3) | BG_CBB(0) | BG_SBB(31) | BG_4BPP,
			&globalViewport);

	memcpy16(pal_bg_bank[1], BG1_palette, 16); // 16 is the number of halfword (16bits) to fill, 1 color is 16 bit
	MapInit(
			&bigmapScene.layer1, // TileMap for the background 1
			BG1_map,
			128,
			128,
			BG1_tiles,
			BG1_tiles_size,
			1,
			// Priority 2, Tiles data -> on the 2nd Charblock, TileMap data -> on the 30th screen entry (32x32 TileMap of 16 bit = 2048bytes = 1 Screen entry)
			BG_PRIO(2) | BG_CBB(1) | BG_SBB(30) | BG_4BPP,
			&globalViewport);

	// Charblocks and screen entries use the same address range, so they can overlaps.
	// To counter that Charblocks are fiiled left to right and screen entries right to left.

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1; // mode 0 with background 0 and 1
}

void BigmapUpdate()
{
	PlayerUpdate(&globalplayer);

	ViewportCenter(
			&globalViewport,
			globalplayer.position.x,
			globalplayer.position.y);

	MapUpdate(&bigmapScene.layer0, &globalViewport);
	MapUpdate(&bigmapScene.layer1, &globalViewport);
}

// Can be use to remove/reset things when changing scene.
void BigmapDestroy()
{
	MapDestroy(&bigmapScene.layer0);
	MapDestroy(&bigmapScene.layer1);
}
