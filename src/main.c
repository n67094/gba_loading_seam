#include <tonc_bios.h>
#include <tonc_core.h>
#include <tonc_input.h>
#include <tonc_irq.h>
#include <tonc_video.h>

#include <stdio.h>
#include <stdlib.h>

#include "./core/core-types.h"

#include "./scenes/bigmap.h"

int main(void)
{
	irq_init(NULL);
	irq_enable(II_VBLANK);

	BigmapInit();

	while (1) {

		VBlankIntrWait();

		key_poll();

		BigmapUpdate();
	}
}
