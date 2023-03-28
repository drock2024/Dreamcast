#define PACK_PIXEL(r, g, b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

#include <kos.h>	//Access KallistiOS
#include <time.h>
#include <stdio.h>	//Standard I/O for console O/P

#include "utils/globals.h"

#include "utils/drawing.h"
#include "utils/input.h"

#include "player.h"


// const int blkWidth=32;	//Block width
// const int blkHeight=32;	//Block height
const int waitTime=2000;	//Delay before checking for user input (microseconds)


int main(void) {
	int quit=0;	//Quit status
	// int blk_xPos=0;	//Block X position
	// int blk_yPos=0;	//Block Y position
 
	pvr_init_defaults();	//Sets up the pvr core with a bunch of default settings
 
	vid_set_mode(DM_320x240, PM_RGB555);	//Set our video mode to 320 x 240, 60Hz and 15-bit (RRRRRGGGGGBBBBB)
 
	// drawBlk(blk_xPos,blk_yPos,blkWidth,blkHeight,0,0,0,0,1,0); 	//Draw green block at current position
	Player player(75, 75);

	//Main loop
	while(!quit) {
		usleep(waitTime);
		player.move();
		// checkForInpUpd(blk_xPos,blk_yPos);	//Check for input from user and update block position if needed
	}
	
	return 0;
}

