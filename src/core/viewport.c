#include <tonc_math.h>
#include <tonc_video.h>

#include "core-types.h"

#include "viewport.h"

void ViewportCenter(Viewport* viewport, int x, int y)
{
	// (>>1 divide by 2)
	ViewportSetPosition(viewport, x - (viewport->size.x >> 1), y - (viewport->size.y >> 1));
}

void ViewportSetPosition(Viewport* viewport, int x, int y)
{
	// Make the viewport stuck on the map edge.
	viewport->position.x = clamp(x, viewport->start.x, viewport->end.x - viewport->size.x);
	viewport->position.y = clamp(y, viewport->start.y, viewport->end.y - viewport->size.y);
}
